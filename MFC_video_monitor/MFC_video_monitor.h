
// MFC_video_monitor.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "ZVideo.h"
#include <memory>

// CMFC_video_monitorApp: 
// �йش����ʵ�֣������ MFC_video_monitor.cpp
//

class CMFC_video_monitorApp : public CWinApp
{
public:
	CMFC_video_monitorApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFC_video_monitorApp theApp;