
// MFC_video_monitorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_video_monitor.h"
#include "MFC_video_monitorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFC_video_monitorDlg 对话框



CMFC_video_monitorDlg::CMFC_video_monitorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFC_VIDEO_MONITOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC_video_monitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_BAR, m_progress);
}

BEGIN_MESSAGE_MAP(CMFC_video_monitorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BEGIN, &CMFC_video_monitorDlg::OnBnClickedBegin)
	ON_BN_CLICKED(IDC_PAUSE, &CMFC_video_monitorDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_CLOSE, &CMFC_video_monitorDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_FAST_FORW, &CMFC_video_monitorDlg::OnBnClickedFastForw)
	ON_BN_CLICKED(IDC_FAST_REW, &CMFC_video_monitorDlg::OnBnClickedFastRew)
	ON_WM_TIMER()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CMFC_video_monitorDlg 消息处理程序

BOOL CMFC_video_monitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	mplayer = &ZVideo::getInstance();
	screen_hwnd = this->GetDlgItem(IDC_SCREEN)->m_hWnd;
	m_progress.SetRange(0, 1000);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFC_video_monitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFC_video_monitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFC_video_monitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFC_video_monitorDlg::OnBnClickedBegin()
{
	if (mplayer->getPlayerState() == libvlc_Playing) {
		OnBnClickedClose();
	}
	CString filter;
	filter = "视频文件(*.avi; *mp4; *mkv; *flv; *rmvb; *wmv; *mpeg; *mov)|*.avi; *mp4; *mkv; *flv; *rmvb; *wmv; *mpeg; *mov||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);
	if (!dlg.DoModal() == IDOK)
	{
		return ;
	}

	CStringA videoUrl;
	videoUrl = dlg.GetPathName();

	mplayer->setUrl(videoUrl);

	mplayer->init(screen_hwnd);
	mplayer->begin();
	isShowDuration = true;
	// 设置定时器
	SetTimer(1, 1000, NULL);
}



void CMFC_video_monitorDlg::OnBnClickedPause()
{
	// TODO: 在此添加控件通知处理程序代码
	// 确定总时长
	CString str;
	GetDlgItemText(IDC_PAUSE, str);

	if (mplayer->getPlayerState()!=libvlc_Playing && str=="暂停") return;

	if (str == "开始") {
		str = "暂停";
	}
	else {
		str = "开始";
	}
	SetDlgItemText(IDC_PAUSE, str);

	mplayer->pause();
}


void CMFC_video_monitorDlg::OnBnClickedClose()
{
	// TODO: 在此添加控件通知处理程序代码
	mplayer->close();
	// 关闭定时器
	KillTimer(1);
	resetRate();
	resetTime();
}


void CMFC_video_monitorDlg::OnBnClickedFastForw()
{
	// TODO: 在此添加控件通知处理程序代码
	mplayer->fastForword();
	resetRate();
}



void CMFC_video_monitorDlg::OnBnClickedFastRew()
{
	// TODO: 在此添加控件通知处理程序代码
	mplayer->fastRewind();
	resetRate();
}

void CMFC_video_monitorDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent==1) {

		CString curtimeStr, durationStr;
		int curtime;
		int duration;

		int thh, tmm, tss;
		curtime = mplayer->getCurrent();
		duration = mplayer->getDuration();

		if (curtime != 0) {
			//change to second
			thh = curtime / 3600;
			tmm = (curtime % 3600) / 60;
			tss = (curtime % 60);
			curtimeStr.Format(_T("%02d:%02d:%02d"), thh, tmm, tss);

			SetDlgItemText(IDC_CURRENT_TIME, curtimeStr);
		}

		// 如果太快获取getDuration会失败，故放到定时器中执行一次
		if (isShowDuration) {
			if (duration != 0) {
				thh = duration / 3600;
				tmm = (duration % 3600) / 60;
				tss = (duration % 60);
				durationStr.Format(_T("%02d:%02d:%02d"), thh, tmm, tss);
				SetDlgItemText(IDC_DURATION_TIME, durationStr);
			}		
			isShowDuration = false;
		}
		int progress = curtime * 1000 / duration ;
		m_progress.SetPos(progress);
	}

	// 播放结束则关闭
	if (mplayer->getPlayerState() == libvlc_Ended)
		OnBnClickedClose();

	CDialogEx::OnTimer(nIDEvent);
}


void CMFC_video_monitorDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (mplayer->getPlayerState() != libvlc_Playing) return;

	if (pScrollBar->GetSafeHwnd() == m_progress.GetSafeHwnd()) {
		float posf = 0.0;
		if (nSBCode == SB_THUMBPOSITION) {
			posf = (float)nPos / 1000.0;
			mplayer->setScroll(posf);
		}
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CMFC_video_monitorDlg::resetTime() {
	CString timeStr;
	timeStr = "00:00:00";

	SetDlgItemText(IDC_CURRENT_TIME, timeStr);
	SetDlgItemText(IDC_DURATION_TIME, timeStr);
}


void CMFC_video_monitorDlg::resetRate() {
	CString strRate;
	strRate.Format(_T("%1.2f"), mplayer->getRate());
	strRate = (CString)"播放速度:" + strRate + (CString)"倍";
	SetDlgItemText(IDC_RATE, strRate);
}