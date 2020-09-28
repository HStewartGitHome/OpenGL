#include "pch.h"
#include "test.h"

// source is derived from
// https://www.cs.rit.edu/~ncs/Courses/570/UserGuide/OpenGLonWin-11.html

// constructor
Test::Test()
{
	m_winWidth = 300;
	m_winHeight = 300;
	m_hGLRC = NULL;
	m_hPalette = NULL;
}

Test::~Test()
{
}

// public funtions

// WM_CREATE
void Test::Create(HWND hWnd)
{
	m_hDC = GetDC(hWnd);
	setupPixelFormat(m_hDC);
	setupPalette(m_hDC);
	m_hGLRC = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, m_hGLRC);
	init();
}

// WM_DESTROY
void Test::Destroy(HWND hWnd)
{
	if (m_hGLRC) 
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_hGLRC);
	}
	if (m_hPalette)
	{
		DeleteObject(m_hPalette);
	}
	ReleaseDC(hWnd, m_hDC);
}

// WM_SIZE
void Test::Size(int width, int height)
{
	if (m_hGLRC)
	{
		m_winWidth = width;
		m_winHeight = height;
		resize();
	}
}
// WM_PALETTECHANGED
void Test::PaletteChange(HWND hWnd,
	WPARAM wParam)
{
	if (m_hGLRC && m_hPalette && (HWND)wParam != hWnd)
	{
		UnrealizeObject(m_hPalette);
		SelectPalette(m_hDC, m_hPalette, FALSE);
		RealizePalette(m_hDC);
		redraw();
	}
}

// WM_QUERYNEWPALETTE
void Test::QueryNewPalette()
{
	if (m_hGLRC && m_hPalette)
	{
		UnrealizeObject(m_hPalette);
		SelectPalette(m_hDC, m_hPalette, FALSE);
		RealizePalette(m_hDC);
		redraw();
	}
}

// WM_PAINT
void Test::Paint(HWND hWnd)
{
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);
	if (m_hGLRC)
	{
		redraw();
	}
	EndPaint(hWnd, &ps);
}

//
// Protected functions

void  Test::init(int winWidth, int winHeight)
{
	if (winWidth == 0)
		winWidth = 300;
	if (winHeight == 0)
		winHeight = 300;

	m_winWidth = winWidth;
	m_winHeight = winHeight;

	/* set viewing projection */
	glMatrixMode(GL_PROJECTION);
	glFrustum(-0.5F, 0.5F, -0.5F, 0.5F, 1.0F, 3.0F);

	/* position viewer */
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(0.0F, 0.0F, -2.0F);

	/* position object */
	glRotatef(30.0F, 1.0F, 0.0F, 0.0F);
	glRotatef(30.0F, 0.0F, 1.0F, 0.0F);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void    Test::redraw(void)
{
	/* clear color and depth buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* draw six faces of a cube */
	glBegin(GL_QUADS);
	glNormal3f(0.0F, 0.0F, 1.0F);
	glVertex3f(0.5F, 0.5F, 0.5F); glVertex3f(-0.5F, 0.5F, 0.5F);
	glVertex3f(-0.5F, -0.5F, 0.5F); glVertex3f(0.5F, -0.5F, 0.5F);

	glNormal3f(0.0F, 0.0F, -1.0F);
	glVertex3f(-0.5F, -0.5F, -0.5F); glVertex3f(-0.5F, 0.5F, -0.5F);
	glVertex3f(0.5F, 0.5F, -0.5F); glVertex3f(0.5F, -0.5F, -0.5F);

	glNormal3f(0.0F, 1.0F, 0.0F);
	glVertex3f(0.5F, 0.5F, 0.5F); glVertex3f(0.5F, 0.5F, -0.5F);
	glVertex3f(-0.5F, 0.5F, -0.5F); glVertex3f(-0.5F, 0.5F, 0.5F);

	glNormal3f(0.0F, -1.0F, 0.0F);
	glVertex3f(-0.5F, -0.5F, -0.5F); glVertex3f(0.5F, -0.5F, -0.5F);
	glVertex3f(0.5F, -0.5F, 0.5F); glVertex3f(-0.5F, -0.5F, 0.5F);

	glNormal3f(1.0F, 0.0F, 0.0F);
	glVertex3f(0.5F, 0.5F, 0.5F); glVertex3f(0.5F, -0.5F, 0.5F);
	glVertex3f(0.5F, -0.5F, -0.5F); glVertex3f(0.5F, 0.5F, -0.5F);

	glNormal3f(-1.0F, 0.0F, 0.0F);
	glVertex3f(-0.5F, -0.5F, -0.5F); glVertex3f(-0.5F, -0.5F, 0.5F);
	glVertex3f(-0.5F, 0.5F, 0.5F); glVertex3f(-0.5F, 0.5F, -0.5F);
	glEnd();

	SwapBuffers(m_hDC);
}

void  Test::setupPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pfd = 
	{
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
	if (pixelFormat == 0) 
	{
		MessageBox(WindowFromDC(hDC), _T("ChoosePixelFormat failed."), _T("Error"),
			MB_ICONERROR | MB_OK);
		exit(1);
	}

	if (SetPixelFormat(m_hDC, pixelFormat, &pfd) != TRUE) 
	{
		MessageBox(WindowFromDC(hDC), _T("SetPixelFormat failed."), _T("Error"),
			MB_ICONERROR | MB_OK);
		exit(1);
	}
}

void Test::resize()
{
	/* set viewport to cover the window */
	glViewport(0, 0, m_winWidth, m_winHeight);
}

void  Test::setupPalette(HDC hDC)
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