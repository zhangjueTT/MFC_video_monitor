
// MFC_video_monitorDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CMFC_video_monitorDlg �Ի���
class CMFC_video_monitorDlg : public CDialogEx
{
// ����
public:
	CMFC_video_monitorDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_VIDEO_MONITOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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


public:
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedFastForw();
	afx_msg void OnBnClickedFastRew();
	afx_msg void resetRate();
	afx_msg void resetTime();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

};
