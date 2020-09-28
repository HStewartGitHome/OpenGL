#pragma once

#include <windows.h>
#include <GL/gl.h>

class GLInfo;

class GetGLInfo
{
public:
	GetGLInfo();
	~GetGLInfo();

	GLInfo* GetCurrentInfo();
	void Create(HWND hWnd);
	void Destroy(HWND hWnd);
	void resize();
	GLInfo* Paint(HWND hWnd);

protected:
	void  setupPixelFormat(HDC hDC);
	void  setupPalette(HDC hDC);
	 BOOL IsCardActive(GLInfo* info,
		CString strCard);

private:
	HDC m_hDC;
	HGLRC m_hGLRC;
	HPALETTE m_hPalette;
	int m_winWidth;
	int m_winHeight;
};
