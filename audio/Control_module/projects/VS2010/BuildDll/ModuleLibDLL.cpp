// TestModuleDLL.cpp : Defines the initialization routines for the DLL.
//

#ifdef WIN32
#include "stdafx.h"
#endif
#include "ModuleLibDLL.h"

#ifdef WIN32
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CTestModuleDLLApp

#ifdef WIN32
BEGIN_MESSAGE_MAP(CTestModuleDLLApp, CWinApp)
END_MESSAGE_MAP()


// CTestModuleDLLApp construction

CTestModuleDLLApp::CTestModuleDLLApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTestModuleDLLApp object

CTestModuleDLLApp theApp;


// CTestModuleDLLApp initialization

BOOL CTestModuleDLLApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
#endif

// End of file.
