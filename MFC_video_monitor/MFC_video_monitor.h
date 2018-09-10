
// MFC_video_monitor.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "ZVideo.h"
#include <memory>

// CMFC_video_monitorApp: 
// 有关此类的实现，请参阅 MFC_video_monitor.cpp
//

class CMFC_video_monitorApp : public CWinApp
{
public:
	CMFC_video_monitorApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CMFC_video_monitorApp theApp;