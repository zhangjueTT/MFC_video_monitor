
// MFC_video_monitorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_video_monitor.h"
#include "MFC_video_monitorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMFC_video_monitorDlg �Ի���



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


// CMFC_video_monitorDlg ��Ϣ�������

BOOL CMFC_video_monitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	mplayer = &ZVideo::getInstance();
	screen_hwnd = this->GetDlgItem(IDC_SCREEN)->m_hWnd;
	m_progress.SetRange(0, 1000);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFC_video_monitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	filter = "��Ƶ�ļ�(*.avi; *mp4; *mkv; *flv; *rmvb; *wmv; *mpeg; *mov)|*.avi; *mp4; *mkv; *flv; *rmvb; *wmv; *mpeg; *mov||";
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
	// ���ö�ʱ��
	SetTimer(1, 1000, NULL);
}



void CMFC_video_monitorDlg::OnBnClickedPause()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ȷ����ʱ��
	CString str;
	GetDlgItemText(IDC_PAUSE, str);

	if (mplayer->getPlayerState()!=libvlc_Playing && str=="��ͣ") return;

	if (str == "��ʼ") {
		str = "��ͣ";
	}
	else {
		str = "��ʼ";
	}
	SetDlgItemText(IDC_PAUSE, str);

	mplayer->pause();
}


void CMFC_video_monitorDlg::OnBnClickedClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	mplayer->close();
	// �رն�ʱ��
	KillTimer(1);
	resetRate();
	resetTime();
}


void CMFC_video_monitorDlg::OnBnClickedFastForw()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	mplayer->fastForword();
	resetRate();
}



void CMFC_video_monitorDlg::OnBnClickedFastRew()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

		// ���̫���ȡgetDuration��ʧ�ܣ��ʷŵ���ʱ����ִ��һ��
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

	// ���Ž�����ر�
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
	strRate = (CString)"�����ٶ�:" + strRate + (CString)"��";
	SetDlgItemText(IDC_RATE, strRate);
}