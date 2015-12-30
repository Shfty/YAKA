#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CMFCYAKAApp, CWinApp)
END_MESSAGE_MAP()

CMFCYAKAApp instance;

CMFCYAKAApp::CMFCYAKAApp()
{

}

BOOL CMFCYAKAApp::InitInstance()
{
	CWinApp::InitInstance();

	CMFCYAKADlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if(nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

