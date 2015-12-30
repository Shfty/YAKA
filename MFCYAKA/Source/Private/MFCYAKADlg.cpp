/*
* YAKA
* Created by Josh Palmer
* 02/11/2015
*/

/* TODO:
* YAKA
* - Refactor connection retry code to allow for tray icon/quit functionality
* - Investigate context less when running exclusive fullscreen applications
* - Rename/reorganise source files (MFCWrapper / YAKA)
* - Improve icon
* - Tray context menu (Show Visualizer / Options / Exit)
* - Setup app ID for DLL -> App messaging
* - Options menu for configuration at runtime
* - Rewrite key rendering to draw into separate texture, expose texture to Renderers?
*
* WaveSim
* - Move keyboard draw into Core, expose CorsairLedColor array to Renderers
* - Batch up pressed keys as vertices and draw them all in one call
* - Multi-pixel key sampling for keyboard draw
* - Rewrite key renderer to use corsair key geometry, higher resolution
* - More intense ripples for shift-modded keys
* - Investigate delta time-driven wave simulation
*
* KeyDLL
* - Fix Alt Gr support (Batch up keys in Core/WaveSim before draw, test for extra Ctrl input?)
*
* New Modules
* - Vector pathing module (Oriented stretched quads + circular endcaps)
* - Crossbeam module (1-d pool sim per-axis)
* - Heatmap module (Fading circles, customizable size, intensity, and decay. press/hold option.)
*
* Public Availability
* - Public Release
* - Support for DLL renderer plugins?
*/

#include "stdafx.h"

using namespace YAKA;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CONNECT_TIMER_ID 1
#define DRAWLOOP_TIMER_ID 2

// Message ID sent by KeyDLL
#define WM_KEYSTROKE (WM_USER + 755)

// tray icon ID
#define ID_SYSTEMTRAY 0x1000

// custom message ID
#define WM_TRAYICON_EVENT (WM_APP + 1)

// tray icon resource ID
#define ID_ICON_TRAY_INITIAL 0x2000

BEGIN_MESSAGE_MAP( CMFCYAKADlg, CDialogEx )
	ON_MESSAGE( WM_TRAYICON_EVENT, OnTrayIconEvent )
	ON_MESSAGE( WM_KEYSTROKE, ProcessKey )
END_MESSAGE_MAP()

GLFWwindow* window = nullptr; // Raw pointer, managed by GLFW
unique_ptr<YAKA::Renderer> renderer;

CMFCYAKADlg::CMFCYAKADlg( CWnd* pParent )
	: CDialogEx( IDD_MFCYAKA_DIALOG, pParent )
{

}

CorsairError error = CorsairGetLastError();
static void CALLBACK TryConnect( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{
	CorsairPerformProtocolHandshake();
	error = CorsairGetLastError();
}

static void CALLBACK DrawLoop( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{
	renderer->Draw();

	glfwSwapInterval( 0 );
	glfwSwapBuffers( window );
}

BOOL CMFCYAKADlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Config::LoadConfig();

	// Initialise OpenGL
	if( !GL::InitGL( window ) )
	{
		MessageBoxA( this->GetSafeHwnd(), "Failed to initialise GL", "Error", MB_OK );
		terminate();
		return FALSE;
	}

	// Install key hook
	installKeyHook( this->GetSafeHwnd() );

	// Connect to corsair device
	TryConnect( NULL, NULL, NULL, NULL );

	// If there's no server, try to connect once per second until we receive another error code
	if( error == CE_ServerNotFound )
	{
		SetTimer( CONNECT_TIMER_ID, 1000, TryConnect );

		while( error != CE_Success )
		{
			// Sit and spin while waiting for connection success
			// TODO: Replace this with native windows equivalent
			glfwPollEvents();
		}

		KillTimer( CONNECT_TIMER_ID );
	}

	// If there's still an error, notify the user and terminate
	if( error != CE_Success )
	{
		MessageBoxA( this->GetSafeHwnd(), corsairErrorToString( error ), "Failed to connect to Corsair device", MB_OK );
		terminate();
		return FALSE;
	}

	CorsairRequestControl( CAM_ExclusiveLightingControl );

	// Create tray icon
	CreateTrayIcon();

	// Setup GLFW minimize callback
	glfwSetWindowIconifyCallback( window, OnMinimize );

	// Create and initialise renderer
	renderer.reset( new WaveSim() );
	
	if( !renderer->Init() )
	{
		MessageBoxA( this->GetSafeHwnd(), "Failed to initialise Renderer", "Error", MB_OK );
		terminate();
		return FALSE;
	}

	// Draw loop, ends when the GLFW window is closed
	SetTimer( DRAWLOOP_TIMER_ID, DWORD( FRAME_TIME ), DrawLoop );

	while( !glfwWindowShouldClose( window ) )
	{
		// Sit and spin to prevent the application from closing
		glfwPollEvents();
	}

	terminate();
	return TRUE;  // return TRUE unless you set the focus to a control
}

// tray icon data
NOTIFYICONDATA m_NID = { 0 };
BOOL CMFCYAKADlg::CreateTrayIcon()
{
	m_NID.cbSize = sizeof( m_NID );

	// set tray icon ID
	m_NID.uID = ID_SYSTEMTRAY;

	// set handle to the window that receives tray icon notifications
	m_NID.hWnd = this->GetSafeHwnd();

	// set message that will be sent from tray icon to the window   
	m_NID.uCallbackMessage = WM_TRAYICON_EVENT;

	// fields that are being set when adding tray icon 
	m_NID.uFlags = NIF_MESSAGE | NIF_ICON;

	// set image
	m_NID.hIcon = LoadIcon( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDR_MAINFRAME ) );

	if( !m_NID.hIcon )
		return FALSE;

	return Shell_NotifyIcon( NIM_ADD, &m_NID );
}

BOOL CMFCYAKADlg::DestroyTrayIcon()
{
	return Shell_NotifyIcon( NIM_DELETE, &m_NID );
}

LRESULT CMFCYAKADlg::ProcessKey( WPARAM wParam, LPARAM lParam )
{
	if( renderer != nullptr )
	{
		renderer->KeyCallback( (int)lParam );
	}
	return 0L;
}

void CMFCYAKADlg::OnMinimize( GLFWwindow* window, int minimized )
{
	if( minimized )
	{
		glfwHideWindow( window );
	}
}

LRESULT CMFCYAKADlg::OnTrayIconEvent( WPARAM wParam, LPARAM lParam )
{
	if( (UINT)wParam != ID_SYSTEMTRAY )
		return 0L;

	switch( (UINT)lParam )
	{
		case WM_MOUSEMOVE:
			break;
		case WM_LBUTTONUP:
			glfwShowWindow( window );
			glfwRestoreWindow( window );
			break;
		case WM_RBUTTONUP:
			glfwDestroyWindow( window );
			break;
		default:
			break;
	}

	return 0L;
}

const char* CMFCYAKADlg::corsairErrorToString( CorsairError error ) const
{
	switch( error )
	{
		case CE_Success:
			return "Success";
		case CE_ServerNotFound:
			return "Server Not Found";
		case CE_NoControl:
			return "No Control";
		case CE_ProtocolHandshakeMissing:
			return "Protocol Handshake Missing";
		case CE_IncompatibleProtocol:
			return "Incompatible Protocol";
		case CE_InvalidArguments:
			return "Invalid Arguments";
		default:
			return "Unknown Error";
	}
}

void CMFCYAKADlg::terminate()
{
	glfwTerminate();
	removeKeyHook();
	DestroyTrayIcon();
	EndDialog( 0 );
}