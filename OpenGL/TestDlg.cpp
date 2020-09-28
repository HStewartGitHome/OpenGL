// TestDlg.cpp : implementation file
//

#include "pch.h"
#include "OpenGL.h"
#include "TestDlg.h"

// TestDlg

IMPLEMENT_DYNAMIC(CTestDlg, CDialogEx)

CTestDlg::CTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TEST, pParent)
{
	test = new Test();
}

CTestDlg::~CTestDlg()
{
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_PALETTECHANGED()
	ON_WM_QUERYNEWPALETTE()
END_MESSAGE_MAP()

// TestDlg message handlers
BOOL CTestDlg::OnInitDialog()
{
	// TODO: Add extra initialization here
	test->Create(this->m_hWnd);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestDlg::OnPaint()
{
	CDialogEx::OnPaint();
	test->Paint(this->m_hWnd);
}

void CTestDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	test->Size(cx, cy);
}


void CTestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	test->Destroy(this->m_hWnd);
}


void CTestDlg::OnPaletteChanged(CWnd* pFocusWnd)
{
	CDialogEx::OnPaletteChanged(pFocusWnd);
	test->PaletteChange(this->m_hWnd, (WPARAM)pFocusWnd);
}


BOOL CTestDlg::OnQueryNewPalette()
{
	// TODO: Add your message handler code here and/or call default
	test->QueryNewPalette();
	return CDialogEx::OnQueryNewPalette();
}
