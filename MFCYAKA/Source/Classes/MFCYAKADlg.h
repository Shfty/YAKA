#pragma once

#include "Core/GL.h"
#include "Core/Renderer.h"
#include "WaveSim/WaveSim.h"

class CMFCYAKADlg : public CDialogEx
{
public:
	CMFCYAKADlg( CWnd* pParent = NULL );

private:
	const char* corsairErrorToString( CorsairError error ) const;
	void terminate();
	void drawLoop( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime );

protected:
	BOOL CreateTrayIcon();
	BOOL DestroyTrayIcon();

	virtual BOOL OnInitDialog();
	virtual LRESULT OnTrayIconEvent( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT ProcessKey( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

	static void OnMinimize( GLFWwindow* window, int minimized );
};