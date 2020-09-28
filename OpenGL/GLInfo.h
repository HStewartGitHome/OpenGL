#pragma once

#include <windows.h>
#include <GL/gl.h>

class GLInfo
{
public:
	GLInfo();
	~GLInfo();

	CString m_strVendor;
	CString m_strVersion;
	CString m_strRenderer;
	CString m_strExtensions;
	CString m_strCardCount;
	CString m_strCard1;
	CString m_strCard2;
	CString m_strActive;
	BOOL m_bCard1Check;
	BOOL m_bCard2Check;
	CString m_strSystemProductName;
	CString m_strSystemManufacturer;
};
