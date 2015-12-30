#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
#define _AFX_ALL_WARNINGS						// turns off MFC's hiding of some common and often safely ignored warning messages

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars

#include <memory>
using std::unique_ptr;

#include "CUESDK.h"
#include "KeyDLL.h"

#include "YAKASettings.h"

#include "MFCYAKA.h"
#include "MFCYAKADlg.h"