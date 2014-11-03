//
// Registry.cpp
//
#include "Stdafx.h"
#include <objbase.h>
#include <assert.h>
#include <shlwapi.h>
#include <string>
#include <windows.h>
#include <locale>
#include <codecvt>
#include "Registry.h"
#ifndef _TEXT
#define _TEXT(str) L##str
#endif
////////////////////////////////////////////////////////
//
// Internal helper functions prototypes
//
//   - These helper functions were borrowed and modifed from
//     Dale Rogerson's book Inside COM.

// Set the given key and its value.
BOOL SetKeyAndValue(const wchar_t* pszPath,
                    const wchar_t* szSubkey,
                    const wchar_t* szValue) ;

// Convert a CLSID into a wchar_t string.
void CLSIDtoString(const CLSID& clsid, 
                 wchar_t* szCLSID,
                 int length) ;

// Delete szKeyChild and all of its descendents.
LONG DeleteKey(HKEY hKeyParent, const wchar_t* szKeyString) ;

////////////////////////////////////////////////////////
//
// Constants
//

// Size of a CLSID as a string
const int CLSID_STRING_SIZE = 39;

/////////////////////////////////////////////////////////
//
// Public function implementation
//

//
// Register the component in the registry.
//
HRESULT RegisterServer(const CLSID& clsid,         // Class ID
                       const wchar_t *szFileName,     // DLL module handle
                       const wchar_t* szProgID,       //   IDs
                       const wchar_t* szDescription,  // Description String
					   const wchar_t* szVerIndProgID) // optional

{
	// Convert the CLSID into a wchar_t.
	wchar_t szCLSID[CLSID_STRING_SIZE*2] ;
	CLSIDtoString(clsid, szCLSID, sizeof(szCLSID)*2) ;

	// Build the key CLSID\\{...}
	wchar_t szKey[64*4] ;
	StrCpyW(szKey, _TEXT("CLSID\\")) ;
	StrCatW(szKey, szCLSID) ;
  
	// Add the CLSID to the registry.
	SetKeyAndValue(szKey, NULL, szDescription) ;

	// Add the server filename subkey under the CLSID key.
	SetKeyAndValue(szKey, _TEXT("InprocServer32"), szFileName) ;

	// Add the ProgID subkey under the CLSID key.
	if (szProgID != NULL) {
		SetKeyAndValue(szKey, _TEXT("ProgID"), szProgID) ;
		SetKeyAndValue(szProgID, _TEXT("CLSID"), szCLSID) ;
	}

	if (szVerIndProgID) {
		// Add the version-independent ProgID subkey under CLSID key.
		SetKeyAndValue(szKey, _TEXT("VersionIndependentProgID"),
					   szVerIndProgID) ;

		// Add the version-independent ProgID subkey under HKEY_CLASSES_ROOT.
		SetKeyAndValue(szVerIndProgID, NULL, szDescription) ; 
		SetKeyAndValue(szVerIndProgID, _TEXT("CLSID"), szCLSID) ;
		SetKeyAndValue(szVerIndProgID, _TEXT("CurVer"), szCLSID) ;

		// Add the versioned ProgID subkey under HKEY_CLASSES_ROOT.
		SetKeyAndValue(szProgID, NULL, szDescription) ; 
		SetKeyAndValue(szProgID, _TEXT("CLSID"), szCLSID) ;
	}

	return S_OK ;
}

//
// Remove the component from the registry.
//
HRESULT UnregisterServer(const CLSID& clsid,      // Class ID
                      const wchar_t* szProgID,       //   IDs
                      const wchar_t* szVerIndProgID) // Programmatic
{
	// Convert the CLSID into a wchar_t.
	wchar_t szCLSID[CLSID_STRING_SIZE] ;
	CLSIDtoString(clsid, szCLSID, sizeof(szCLSID)) ;

	// Build the key CLSID\\{...}
	wchar_t szKey[64] ;
	StrCpyW(szKey, _TEXT("CLSID\\"));
	StrCatW(szKey, szCLSID) ;

	// Delete the CLSID Key - CLSID\{...}
	LONG lResult = DeleteKey(HKEY_CLASSES_ROOT, szKey) ;

	// Delete the version-independent ProgID Key.
	if (szVerIndProgID != NULL)
		lResult = DeleteKey(HKEY_CLASSES_ROOT, szVerIndProgID) ;

	// Delete the ProgID key.
	if (szProgID != NULL)
		lResult = DeleteKey(HKEY_CLASSES_ROOT, szProgID) ;

	return S_OK ;
}

///////////////////////////////////////////////////////////
//
// Internal helper functions
//

// Convert a CLSID to a wchar_t string.
void CLSIDtoString(const CLSID& clsid,
                 wchar_t* szCLSID,
                 int length)
{
	assert(length >= CLSID_STRING_SIZE) ;
	// Get CLSID
	LPOLESTR wszCLSID = NULL ;
	HRESULT hr = StringFromCLSID(clsid, &wszCLSID) ;
	assert(SUCCEEDED(hr)) ;
	StrCpyW(szCLSID, wszCLSID);
	// Covert from wide wchar_tacters to non-wide.
	// Free memory.
	CoTaskMemFree(wszCLSID) ;
}

//
// Delete a key and all of its descendents.
//
LONG DeleteKey(HKEY hKeyParent,           // Parent of key to delete
               const wchar_t* lpszKeyChild)  // Key to delete
{
	// Open the child.
	HKEY hKeyChild ;
	LONG lRes = RegOpenKeyEx(hKeyParent, lpszKeyChild, 0,
	                         KEY_ALL_ACCESS, &hKeyChild) ;
	if (lRes != ERROR_SUCCESS)
	{
		return lRes ;
	}

	// Enumerate all of the decendents of this child.
	FILETIME time ;
	wchar_t szBuffer[256] ;
	DWORD dwSize = 256 ;
	while (RegEnumKeyEx(hKeyChild, 0, szBuffer, &dwSize, NULL,
	                    NULL, NULL, &time) == S_OK)
	{
		// Delete the decendents of this child.
		lRes = DeleteKey(hKeyChild, szBuffer) ;
		if (lRes != ERROR_SUCCESS)
		{
			// Cleanup before exiting.
			RegCloseKey(hKeyChild) ;
			return lRes;
		}
		dwSize = 256 ;
	}

	// Close the child.
	RegCloseKey(hKeyChild) ;

	// Delete this child.
	return RegDeleteKey(hKeyParent, lpszKeyChild) ;
}

//
// Create a key and set its value.
//
BOOL SetKeyAndValue(const wchar_t* szKey,
                    const wchar_t* szSubkey,
                    const wchar_t* szValue)
{
	HKEY hKey;
	wchar_t szKeyBuf[1024] ;

	// Copy keyname into buffer.
	StrCpyW(szKeyBuf, szKey) ;

	// Add subkey name to buffer.
	if (szSubkey != NULL)
	{
		StrCatW(szKeyBuf, _TEXT("\\"));
		StrCatW(szKeyBuf, szSubkey ) ;
	}

	// Create and open key and subkey.
	long lResult = RegCreateKeyEx(HKEY_CLASSES_ROOT ,
	                              szKeyBuf, 
	                              0, NULL, REG_OPTION_NON_VOLATILE,
	                              KEY_ALL_ACCESS, NULL, 
	                              &hKey, NULL) ;
	if (lResult != ERROR_SUCCESS)
	{
		return FALSE ;
	}

	// Set the Value.
	if (szValue != NULL)
	{
		RegSetValueEx(hKey, NULL, 0, REG_SZ, 
		              (BYTE*)szValue, (std::wstring(szValue).size()+1)*2) ;
	}

	RegCloseKey(hKey) ;
	return TRUE ;
}
