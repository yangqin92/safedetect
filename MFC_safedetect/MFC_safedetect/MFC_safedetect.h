
// MFC_safedetect.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFC_safedetectApp: 
// �йش����ʵ�֣������ MFC_safedetect.cpp
//

class CMFC_safedetectApp : public CWinApp
{
public:
	CMFC_safedetectApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFC_safedetectApp theApp;