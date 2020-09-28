#pragma once

#include <windows.h>
#include <GL/gl.h>

// source is derived from
// https://www.cs.rit.edu/~ncs/Courses/570/UserGuide/OpenGLonWin-11.html

class Test
{
public:
	Test();
	virtual ~Test();

	void Create(HWND hWnd);
	void Destroy(HWND hWnd);
	void Size(int width, int height);
	void PaletteChange(HWND hWnd,
		WPARAM wParam);
	void QueryNewPalette();
	void Paint(HWND hWnd);

protected:
	void  init(int winWidth = 0,
		int winHeight = 0);

	void  redraw();
	void  resize();
	void  setupPalette(HDC hDC);
	void  setupPixelFormat(HDC hDC);

private:
	int m_winWidth;
	int m_winHeight;
	HDC m_hDC;
	HGLRC m_hGLRC;
	HPALETTE m_hPalette;
};
