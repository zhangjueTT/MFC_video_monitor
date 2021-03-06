
// MFC_video_monitorDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include <map>
#include "afxwin.h"


// CMFC_video_monitorDlg 对话框
class CMFC_video_monitorDlg : public CDialogEx
{
// 构造
public:
	CMFC_video_monitorDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_VIDEO_MONITOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBegin();


private:
	ZVideo* mplayer;
	HWND screen_hwnd = NULL;
	CSliderCtrl m_progress;
	bool isShowDuration;
	bool isFullSceen{false};

	WINDOWPLACEMENT m_saveLocation;
	CWnd* m_pScreen;
	CWnd* m_pScreenParant;

	CListBox videoList;
	CSliderCtrl m_volume;
	int storeVolume{ 0 };
	std::map<CString, CStringA> fileMap;

public:
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedFastForw();
	afx_msg void OnBnClickedFastRew();
	afx_msg void resetRate();
	afx_msg void resetTime();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void scanCurrentDir(CString strDir);
	afx_msg void OnLbnDblclkVideoList();
	afx_msg void OnBnClickedNoVolumn();
	afx_msg void initVolumn();
	afx_msg void OnBnClickedStoreImage();
	afx_msg void OnBnClickedStoreVideoFrag();
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedFullScreen();
};
