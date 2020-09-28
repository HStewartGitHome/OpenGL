#include "pch.h"
#include "CSysInfo.h"

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

void CSysInfo::QueryKey(HKEY hKey,
	DWORD& dwNum,
	CString chKeyName[])
{
	TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
	DWORD    cbName;                   // size of name string
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name
	DWORD    cchClassName = MAX_PATH;  // size of class string
	DWORD    cSubKeys = 0;               // number of subkeys
	DWORD    cbMaxSubKey;              // longest subkey size
	DWORD    cchMaxClass;              // longest class string
	DWORD    cValues;              // number of values for key
	DWORD    cchMaxValue;          // longest value name
	DWORD    cbMaxValueData;       // longest value data
	DWORD    cbSecurityDescriptor; // size of security descriptor
	FILETIME ftLastWriteTime;      // last write time

	DWORD i, retCode;

	TCHAR  achValue[MAX_VALUE_NAME];
	DWORD cchValue = MAX_VALUE_NAME;

	// Get the class name and the value count.
	retCode = RegQueryInfoKey(
		hKey,                    // key handle
		achClass,                // buffer for class name
		&cchClassName,           // size of class string
		NULL,                    // reserved
		&cSubKeys,               // number of subkeys
		&cbMaxSubKey,            // longest subkey size
		&cchMaxClass,            // longest class string
		&cValues,                // number of values for this key
		&cchMaxValue,            // longest value name
		&cbMaxValueData,         // longest value data
		&cbSecurityDescriptor,   // security descriptor
		&ftLastWriteTime);       // last write time

	// Enumerate the subkeys, until RegEnumKeyEx fails.

	if (cSubKeys)
	{
		printf("\nNumber of subkeys: %d\n", cSubKeys);

		dwNum = cSubKeys;
		for (i = 0; i < cSubKeys; i++)
		{
			cbName = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx(hKey, i,
				achKey,
				&cbName,
				NULL,
				NULL,
				NULL,
				&ftLastWriteTime);
			if (retCode == ERROR_SUCCESS)
			{
				_tprintf(TEXT("(%d) %s\n"), i + 1, achKey);
				chKeyName[i] = achKey;
			}
		}
	}
}

BOOL CSysInfo::QueryCardDesc(HKEY hKey,
	CString& strDesc)
{
	BOOL Result = FALSE;
	TCHAR sz[512] = { 0 };
	long lResult;
	DWORD size = 0, type = 0;

	size = sizeof(sz);
	lResult = RegQueryValueEx(hKey, TEXT("DeviceDesc"), 0, &type, (LPBYTE)sz, &size);
	if (lResult != ERROR_FILE_NOT_FOUND)
	{
		strDesc = sz;
		Result = TRUE;
	}
	return Result;
}

void CSysInfo::QueryCardKeyDesc(DWORD  dwKeyNum,
	CString chKeyName[],
	DWORD& dwCardNum,
	CString chCardName[])
{
	int index;
	dwCardNum = 0;
	CString strKey, strCard, str, strToken;
	HKEY hTestKey;

	for (index = 0; index < dwKeyNum; index++)
	{
		strKey = TEXT("SYSTEM\\CurrentControlSet\\Control\\Video\\") + chKeyName[index] + TEXT("\\VIDEO");

		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			strKey,
			0,
			KEY_READ,
			&hTestKey) == ERROR_SUCCESS
			)
		{
			if (QueryCardDesc(hTestKey, strCard))
			{
				str = strCard;
				int curPos = 0;
				strToken = str.Tokenize(_T(";"), curPos);
				if (strToken.GetLength() > 0)
					strToken = str.Tokenize(_T(";"), curPos);
			}
			chCardName[dwCardNum++] = strToken;
		}

		RegCloseKey(hTestKey);
	}
}

void CSysInfo::GetKeys(DWORD& dwCardNum,
	CString chCardNames[])
{
	HKEY hTestKey;
	DWORD dwNum = 0;
	CString chKeyNames[20];

	dwCardNum = 0;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		TEXT("SYSTEM\\CurrentControlSet\\Control\\Video"),
		0,
		KEY_READ,
		&hTestKey) == ERROR_SUCCESS
		)
	{
		QueryKey(hTestKey, dwNum, chKeyNames);
		QueryCardKeyDesc(dwNum, chKeyNames, dwCardNum, chCardNames);
	}

	RegCloseKey(hTestKey);
}

void CSysInfo::GetSystemBiosField(CString strField,
							      CString& strName)
{
	HKEY hTestKey;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		TEXT("HARDWARE\\DESCRIPTION\\System\\BIOS"),
		0,
		KEY_READ,
		&hTestKey) == ERROR_SUCCESS
		)
	{
		TCHAR sz[512] = { 0 };
		long lResult;
		DWORD size = 0, type = 0;

		size = sizeof(sz);
		lResult = RegQueryValueEx(hTestKey, strField, 0, &type, (LPBYTE)sz, &size);
		if (lResult != ERROR_FILE_NOT_FOUND)
			strName = sz;
	}

	RegCloseKey(hTestKey);
}