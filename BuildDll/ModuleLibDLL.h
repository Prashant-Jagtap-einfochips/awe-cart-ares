// TestModuleDLL.h : main header file for the TestModuleDLL DLL
//

#ifdef WIN32
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#endif

#include "Framework.h"

// CTestModuleDLLApp
// See TestModuleDLL.cpp for the implementation of this class
//

#ifdef WIN32
class CTestModuleDLLApp : public CWinApp
{
public:
	CTestModuleDLLApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
#endif

#ifdef	__cplusplus
extern "C" {
#endif

// Imports
extern void awe_fwRegisterModules(ClassDescriptor *pModuleTable[], int nModules);

// Exports.
DLLSYMBOL const char *ModuleDepend();

DLLSYMBOL void RegisterModules();

DLLSYMBOL const char *GetSchema();

#ifdef	__cplusplus
}
#endif

// End of file.
