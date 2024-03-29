#pragma once

#define _WIN32_WINNT 0x0601 // Windows 7

#include <tchar.h>

#include <windows.h>
#include <mfapi.h>
#include <mfidl.h>
#include <Mfreadwrite.h>
#include <mferror.h>
#include <codecapi.h>
#include <shlwapi.h>
#include <gdiplus.h>
#include <comdef.h>

#include <atlbase.h>

#include <vector>
#include <string>

#include <iostream>
#include <stdio.h>

using namespace Gdiplus;

template <class T> void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}
