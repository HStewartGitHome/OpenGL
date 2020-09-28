#include "pch.h"
#include "GetGLInfo.h"
#include "GLInfo.h"
#include "CSysInfo.h"

GetGLInfo::GetGLInfo()
{
	m_winWidth = 300;
	m_winHeight = 300;
	m_hGLRC = NULL;
}

GetGLInfo::~GetGLInfo()
{
}

GLInfo* GetGLInfo::GetCurrentInfo()
{
	GLInfo* info = new GLInfo();
	DWORD dwNum = 0;
	CString strNames[10];
	CString str;

	info->m_strVendor = glGetString(GL_VENDOR);
	info->m_strVersion = glGetString(GL_VERSION);
	info->m_strRenderer = glGetString(GL_RENDERER);
	info->m_strExtensions = glGetString(GL_EXTENSIONS);

	CSysInfo::GetKeys(dwNum, strNames);
	info->m_strCardCount.Format(_T("%ld"), dwNum);
	info->m_strCard1 = strNames[0];
	info->m_strCard2 = strNames[1];

	if (IsCardActive( info, info->m_strCard1) )
	{
		info->m_strActive = info->m_strCard1;
		info->m_bCard1Check = TRUE;
	}
	if (IsCardActive(info, info->m_strCard2))
	{
		info->m_strActive = info->m_strCard2;
		info->m_bCard2Check = TRUE;
	}
	CSysInfo::GetSystemBiosField(_T("SystemManufacturer"), str);
	info->m_strSystemManufacturer = str;
	CSysInfo::GetSystemBiosField(_T("SystemProductName"), str);
	info->m_strSystemProductName = str;

	return info;
}

BOOL GetGLInfo::IsCardActive(GLInfo *info,
	CString strCard)
{
	BOOL bResult = FALSE;

	if (info->m_strRenderer.CompareNoCase(strCard) == 0)
		bResult = TRUE;
	else
	{
		// currently only use Vendor if no direct match
		int curPos = 0;
		CString strVendor = info->m_strVendor.Tokenize(_T(" "), curPos);
		if (strCard.Find(info->m_strVendor) != -1)
			bResult = TRUE;
		else if (strCard.Find(strVendor) != -1)
			bResult = TRUE;
	}

	return bResult;
}

GLInfo* GetGLInfo::Paint(HWND hWnd)
{
	GLInfo* info = NULL;

	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);
	if (m_hGLRC)
	{
		info = GetCurrentInfo();
	}
	EndPaint(hWnd, &ps);

	return info;
}

void GetGLInfo::Create(HWND hWnd)
{
	m_hDC = GetDC(hWnd);
	setupPixelFormat(m_hDC);
	setupPalette(m_hDC);
	m_hGLRC = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, m_hGLRC);
}

void GetGLInfo::Destroy(HWND hWnd)
{
	if (m_hGLRC)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_hGLRC);
	}

	ReleaseDC(hWnd, m_hDC);
}

void GetGLInfo::resize()
{
	/* set viewport to cover the window */
	glViewport(0, 0, m_winWidth, m_winHeight);
}

void  GetGLInfo::setupPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  /* size */
		1,                              /* version */
		PFD_SUPPORT_OPENGL |
		PFD_DRAW_TO_WINDOW |
		PFD_DOUBLEBUFFER,               /* support double-buffering */
		PFD_TYPE_RGBA,                  /* color type */
		16,                             /* prefered color depth */
		0, 0, 0, 0, 0, 0,               /* color bits (ignored) */
		0,                              /* no alpha buffer */
		0,                              /* alpha bits (ignored) */
		0,                              /* no accumulation buffer */
		0, 0, 0, 0,                     /* accum bits (ignored) */
		16,                             /* depth buffer */
		0,                              /* no stencil buffer */
		0,                              /* no auxiliary buffers */
		PFD_MAIN_PLANE,                 /* main layer */
		0,                              /* reserved */
		0, 0, 0,                        /* no layer, visible, damage masks */
	};
	int pixelFormat;

	pixelFormat = ChoosePixelFormat(m_hDC, &pfd);
	if (pixelFormat == 0) {
		MessageBox(WindowFromDC(hDC), _T("ChoosePixelFormat failed."), _T("Error"),
			MB_ICONERROR | MB_OK);
		exit(1);
	}

	if (SetPixelFormat(m_hDC, pixelFormat, &pfd) != TRUE) {
		MessageBox(WindowFromDC(hDC), _T("SetPixelFormat failed."), _T("Error"),
			MB_ICONERROR | MB_OK);
		exit(1);
	}
}

void  GetGLInfo::setupPalette(HDC hDC)
{
	int pixelFormat = GetPixelFormat(hDC);
	PIXELFORMATDESCRIPTOR pfd;
	LOGPALETTE* pPal;
	int paletteSize, redMask, blueMask, greenMask, i;

	DescribePixelFormat(hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	if (pfd.dwFlags & PFD_NEED_PALETTE)
	{
		paletteSize = 1 << pfd.cColorBits;
	}
	else
	{
		return;
	}

	pPal = (LOGPALETTE*)
		malloc(sizeof(LOGPALETTE) + paletteSize * sizeof(PALETTEENTRY));
	pPal->palVersion = 0x300;
	pPal->palNumEntries = paletteSize;

	// build a simple RGB color palette

	redMask = (1 << pfd.cRedBits) - 1;
	greenMask = (1 << pfd.cGreenBits) - 1;
	blueMask = (1 << pfd.cBlueBits) - 1;

	for (i = 0; i < paletteSize; ++i)
	{
		pPal->palPalEntry[i].peRed =
			(((i >> pfd.cRedShift) & redMask) * 255) / redMask;
		pPal->palPalEntry[i].peGreen =
			(((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
		pPal->palPalEntry[i].peBlue =
			(((i >> pfd.cBlueShift) & blueMask) * 255) / blueMask;
		pPal->palPalEntry[i].peFlags = 0;
	}

	m_hPalette = CreatePalette(pPal);
	free(pPal);

	if (m_hPalette)
	{
		SelectPalette(hDC, m_hPalette, FALSE);
		RealizePalette(hDC);
	}
}