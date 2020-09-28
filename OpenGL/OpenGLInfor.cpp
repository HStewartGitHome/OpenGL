// OpenGLInfor.cpp : implementation file
//

#include "pch.h"
#include "OpenGL.h"
#include "OpenGLInfor.h"
#include "afxdialogex.h"
#include "GLInfo.h"
#include "GetGLInfo.h"

// OpenGLInfor dialog

IMPLEMENT_DYNAMIC(COpenGLInfo, CDialogEx)

COpenGLInfo::COpenGLInfo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OPENGL_INFO, pParent)
	, m_Vendor(_T(""))
	, m_Version(_T(""))
	, m_Renderer(_T(""))
	, m_Extensions(_T(""))
	, m_CardCount(_T(""))
	, m_Card1(_T(""))
	, m_Card2(_T(""))
	, m_Active(_T(""))
	, m_Card1Check(FALSE)
	, m_Card2Check(FALSE)
	, m_Manufacturer(_T(""))
	, m_ProductName(_T(""))
{
	pGetGLInfo = new GetGLInfo();
}

COpenGLInfo::~COpenGLInfo()
{
	if (pGetGLInfo)
		delete(pGetGLInfo);
}

void COpenGLInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_VEMDOR, m_Vendor);
	DDX_Text(pDX, IDC_VERSION, m_Version);
	DDX_Text(pDX, IDC_RENDERER, m_Renderer);
	DDX_Text(pDX, IDC_EXTENSIONS, m_Extensions);
	DDX_Text(pDX, IDC_NOOFCARDS, m_CardCount);
	DDX_Text(pDX, IDC_CARDS1, m_Card1);
	DDX_Text(pDX, IDC_CARDS2, m_Card2);
	DDX_Text(pDX, IDC_ACTIVE, m_Active);
	DDX_Check(pDX, IDC_CHECK1, m_Card1Check);
	DDX_Check(pDX, IDC_CHECK2, m_Card2Check);
	DDX_Text(pDX, IDC_MANUFACTURER, m_Manufacturer);
	DDX_Text(pDX, IDC_PRODUCTNAME, m_ProductName);
}

BEGIN_MESSAGE_MAP(COpenGLInfo, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// OpenGLInfor message handlers
// TestDlg message handlers
BOOL COpenGLInfo::OnInitDialog()
{
	// TODO: Add extra initialization here
	pGetGLInfo->Create(this->m_hWnd);
	pGetGLInfo->resize();
	m_bFirstTime = TRUE;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COpenGLInfo::OnPaint()
{
	CDialogEx::OnPaint();

	if (m_bFirstTime)
	{
		GLInfo* pInfo = pGetGLInfo->Paint(this->m_hWnd);

		m_Vendor = pInfo->m_strVendor;
		m_Version = pInfo->m_strVersion;
		m_Renderer = pInfo->m_strRenderer;
		m_Extensions = pInfo->m_strExtensions;
		m_Active = pInfo->m_strActive;
		m_Card1 = pInfo->m_strCard1;
		m_Card2 = pInfo->m_strCard2;
		m_Card1Check = pInfo->m_bCard1Check;
		m_Card2Check = pInfo->m_bCard2Check;
		m_CardCount = pInfo->m_strCardCount;
		m_Manufacturer = pInfo->m_strSystemManufacturer;
		m_ProductName = pInfo->m_strSystemProductName;

		m_bFirstTime = FALSE;
		UpdateData(FALSE);
		UpdateWindow();
	}
}