#pragma once

#define _WIN32_WINNT 0x0601 // Windows 7

#ifndef STRICT
#define STRICT
#endif

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#define ATL_NO_ASSERT_ON_DESTROY_NONEXISTENT_WINDOW

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

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
#include <atlcom.h>
#include <atlctl.h>

#include <vector>
#include <string>

#include <iostream>
#include <stdio.h>

#include "resource.h"

using namespace Gdiplus;
using namespace ATL;

template <class T> void SafeRelease(T **ppT)
{
   if (*ppT)
   {
      (*ppT)->Release();
      *ppT = NULL;
   }
}
