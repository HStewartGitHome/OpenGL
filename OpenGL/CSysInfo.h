#pragma once
class CSysInfo
{
public:
	static void QueryKey(HKEY hKey,
		DWORD& dwNum,
		CString chKeyName[]);
	static BOOL QueryCardDesc(HKEY hKey,
		CString& strDesc);
	static void QueryCardKeyDesc(DWORD  dwKeyNum,
		CString chKeyName[],
		DWORD& dwCardNum,
		CString chCardName[]);

	static void GetKeys(DWORD& dwCardNum,
		CString chCardNames[]);
	static void GetSystemBiosField(CString strField,
		CString& strName);

};
