
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
	DDX_Control(pDX, IDC_VIDEO_LIST, videoList);
	DDX_Control(pDX, IDC_VOLUMN_BAR, m_volume);
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
	ON_LBN_DBLCLK(IDC_VIDEO_LIST, &CMFC_video_monitorDlg::OnLbnDblclkVideoList)
	ON_BN_CLICKED(IDC_NO_VOLUMN, &CMFC_video_monitorDlg::OnBnClickedNoVolumn)
	ON_BN_CLICKED(IDC_STORE_IMAGE, &CMFC_video_monitorDlg::OnBnClickedStoreImage)
	ON_BN_CLICKED(IDC_STORE_VIDEO_FRAG, &CMFC_video_monitorDlg::OnBnClickedStoreVideoFrag)
	ON_BN_CLICKED(IDC_FULL_SCREEN, &CMFC_video_monitorDlg::OnBnClickedFullScreen)
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
	m_pScreen = this->GetDlgItem(IDC_SCREEN);
	m_progress.SetRange(0, 1000);
	m_volume.SetRange(0, 100);


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
	if (mplayer->getPlayerState() != libvlc_NothingSpecial) {
		OnBnClickedClose();
	}
	CString filter;
	filter = "��Ƶ�ļ�(*.avi; *mp4; *mkv; *flv; *rmvb; *wmv; *mpeg; *mov)|*.avi; *mp4; *mkv; *flv; *rmvb; *wmv; *mpeg; *mov||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);
	if (dlg.DoModal() == IDOK)
	{

		CStringA videoUrl;
		videoUrl = dlg.GetPathName();

		fileMap.clear();
		// ������ǰ�ļ���
		scanCurrentDir((CString)videoUrl);

		int i = 0;
		for (auto item : fileMap) {
			videoList.InsertString(i++, item.first);
		}

		mplayer->setUrl(videoUrl);

		mplayer->init(screen_hwnd);
		mplayer->begin();

		// C��ʼ������
		initVolumn();

		// ���ö�ʱ��
		isShowDuration = true;
		SetTimer(1, 1000, NULL);
	}

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

	mplayer->pause();
	SetDlgItemText(IDC_PAUSE, str);
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
	if (mplayer->getPlayerState() == libvlc_NothingSpecial) return;

	if (pScrollBar->GetSafeHwnd() == m_progress.GetSafeHwnd()) {
		double posf = 0.0;
		if (nSBCode == SB_THUMBPOSITION) {
			posf = (double)nPos / 1000.0;
			mplayer->setProgress(posf);
		}

	}

	if (pScrollBar->GetSafeHwnd() == m_volume.GetSafeHwnd()) {
		if (nSBCode == SB_THUMBPOSITION) {
			mplayer->setVolumn(nPos);
			CString volume;
			volume.Format(_T("%d"), nPos);
			SetDlgItemText(IDC_STATIC_VOLUME, volume);
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


void CMFC_video_monitorDlg::initVolumn()
{
	CString volume;
	int vnum = mplayer->getVolumn();
	m_volume.SetPos(vnum);
	volume.Format(_T("%d"), vnum);
	SetDlgItemText(IDC_STATIC_VOLUME, volume);
}

// ������ǰ�ļ���
void CMFC_video_monitorDlg::scanCurrentDir(CString filter)
{
	int n = filter.ReverseFind('\\');
	if (n >= 0)
		filter = filter.Left(n + 1);

	fileMap.clear();	
	CString suffix[8] = { (CString)"*.avi", (CString)"*mp4", (CString)"*mkv", (CString)"*flv", (CString)"*rmvb", (CString)"*wmv", (CString)"mpeg", (CString)"*mov" };

	CFileFind finder;
	CString path, name, filterPath;
	for (int i = 0; i < 8; i++) 
	{
		filterPath = filter + suffix[i];
		BOOL bWorking = finder.FindFile(filterPath);
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			path = finder.GetFilePath();
			name = finder.GetFileName();
			fileMap[name] = (CStringA)path;
		}
	}

	finder.Close();
}

// list˫���¼�
void CMFC_video_monitorDlg::OnLbnDblclkVideoList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex;
	CString name;
	nIndex = videoList.GetCurSel();
	if (nIndex < 0) return ;

	// ���������
	videoList.GetText(nIndex, name);
	if (!fileMap[name]) return;
	
	// ��Ϊ���� �����µ���Ƶ
	if (mplayer->getPlayerState() != libvlc_NothingSpecial) {
		OnBnClickedClose();
	}

	mplayer->setUrl(fileMap[name]);
	mplayer->init(screen_hwnd);
	mplayer->begin();

	// C��ʼ������
	initVolumn();

	// ���ö�ʱ��
	isShowDuration = true;
	SetTimer(1, 1000, NULL);

}


// ����
void CMFC_video_monitorDlg::OnBnClickedNoVolumn()
{
	CString str;
	GetDlgItemText(IDC_NO_VOLUME, str);

	if (mplayer->getPlayerState() != libvlc_Playing && str == "����") return;

	if (str == "����") {
		storeVolume = mplayer->getVolumn();
		mplayer->setVolumn(0);
		str = "ȡ������";
	}
	else {
		mplayer->setVolumn(storeVolume);
		str = "����";
	}

	SetDlgItemText(IDC_NO_VOLUME, str);
}



void CMFC_video_monitorDlg::OnBnClickedStoreImage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (mplayer->getPlayerState() == libvlc_NothingSpecial) {
		return ;
	}
	CString filter;
	filter = "��ͼ(*.jpg; *.bmp; *.png)|*.jpg; *.bmp; *.png||";
	CFileDialog dlg(FALSE, _T("png"), _T("no_name"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter);
	if (dlg.DoModal() == IDOK)
	{
		CStringA filePath;
		filePath = dlg.GetPathName();
		mplayer->snapshot(filePath);
	}

}

BOOL CMFC_video_monitorDlg::PreTranslateMessage(MSG* pMsg)
{
	CString volume;
		//�ж��Ƿ��¼���Enter��
	switch (pMsg->wParam) {
		// ���ȿ���
	case VK_RIGHT:
		mplayer->setProgress( (mplayer->getCurrent() + 5.0) / (double)mplayer->getDuration() );
		break;
	case VK_LEFT:
		mplayer->setProgress( (mplayer->getCurrent() + 5.0) / (double)mplayer->getDuration() );
		break;
		// ��������
	case VK_UP:
		storeVolume = mplayer->getVolumn() + 5;
		if (storeVolume > 150) storeVolume = 150;
		mplayer->setVolumn(storeVolume);
		m_volume.SetPos(storeVolume);
		volume.Format(_T("%d"), storeVolume);
		SetDlgItemText(IDC_STATIC_VOLUME, volume);
		break;
	case VK_DOWN:
		storeVolume = mplayer->getVolumn() - 5;
		if (storeVolume < 0) storeVolume = 0;
		mplayer->setVolumn(storeVolume);
		m_volume.SetPos(storeVolume);
		volume.Format(_T("%d"), storeVolume);
		SetDlgItemText(IDC_STATIC_VOLUME, volume);
		break;
		// ��ʼ����ͣ
	case VK_SPACE:
		OnBnClickedPause();
		break;
		// ȫ��ʱ��ָ���ȫ��
	case VK_SHIFT:
		if (isFullSceen) {
			m_pScreenParant->ShowWindow(SW_SHOW);
			m_pScreen->SetParent(m_pScreenParant);
			m_pScreen->SetWindowPlacement(&m_saveLocation);//��ԭ

			isFullSceen = false;
		}
		break;
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CMFC_video_monitorDlg::OnBnClickedFullScreen()
{
	if (mplayer->getPlayerState() == libvlc_NothingSpecial) {
		return;
	}
	if (isFullSceen)//�ָ�
	{
		m_pScreenParant->ShowWindow(SW_SHOW);
		m_pScreen->SetParent(m_pScreenParant);
		m_pScreen->SetWindowPlacement(&m_saveLocation);//��ԭ

		isFullSceen = false;
	}
	else//ȫ��
	{
		//�õ���Ļ��С
		int cx = ::GetSystemMetrics(SM_CXSCREEN);
		int cy = ::GetSystemMetrics(SM_CYSCREEN);

		// ��ö�λ
		m_pScreen->GetWindowPlacement(&m_saveLocation);

		m_pScreenParant = m_pScreen->SetParent(NULL);
		m_pScreenParant->ShowWindow(SW_HIDE);						//ԭ������ ����
		m_pScreen->MoveWindow(0, 0, cx, cy);						//�ƶ�����
		isFullSceen = true;
	}
}


void CMFC_video_monitorDlg::OnBnClickedStoreVideoFrag()
{

}
