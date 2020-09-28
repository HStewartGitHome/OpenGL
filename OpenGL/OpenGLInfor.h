#pragma once
#include "GetGLInfo.h"

// OpenGLInfor dialog

class COpenGLInfo : public CDialogEx
{
	DECLARE_DYNAMIC(COpenGLInfo)

public:
	COpenGLInfo(CWnd* pParent = nullptr);   // standard constructor
	virtual ~COpenGLInfo();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENGL_INFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	GetGLInfo* pGetGLInfo;
	BOOL	   m_bFirstTime;

public:
	CString m_Vendor;
	CString m_Version;
	CString m_Renderer;
	CString m_Extensions;
	CString m_CardCount;
	CString m_Card1;
	CString m_Card2;
	CString m_Active;
	BOOL m_Card1Check;
	BOOL m_Card2Check;
	CString m_Manufacturer;
	CString m_ProductName;
};
