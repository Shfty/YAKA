#include "stdafx.h"
#include <iostream>

#pragma data_seg(".HOOKDATA") //Shared data among all instances.
HHOOK hook = NULL;
HWND hwnd = NULL;
#pragma data_seg()

#pragma comment(linker, "/SECTION:.HOOKDATA,RWS") //linker directive

HINSTANCE hinstance = NULL;

BOOL APIENTRY DllMain( HMODULE hModule,
					   DWORD  ul_reason_for_call,
					   LPVOID lpReserved
					   )
{
	switch( ul_reason_for_call )
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}

	hinstance = (HINSTANCE)hModule;
	hook = NULL;

	return TRUE;
}

KEYDLL_API void installKeyHook( HWND h )
{
	hook = NULL;
	hwnd = h;
	hook = SetWindowsHookEx( WH_KEYBOARD_LL, hookProc, hinstance, NULL );
}

KEYDLL_API void removeKeyHook()
{
	UnhookWindowsHookEx( hook );
}

KEYDLL_API LRESULT CALLBACK hookProc( int nCode, WPARAM wParam, LPARAM lParam )
{
	if( nCode >= 0 )
	{
		if( wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN ) //Check whether key was pressed(not released).
		{
			KBDLLHOOKSTRUCT* keyInfo = reinterpret_cast<KBDLLHOOKSTRUCT*>( lParam );
			DWORD vkCode = keyInfo->vkCode;

			if( keyInfo->flags & LLKHF_EXTENDED )
			{
				if( vkCode == VK_RMENU )
				{
				}
				else if( vkCode == VK_RETURN )
				{
					// Keypad Enter pressed, inject unused virtual keycode to differentiate
					vkCode = 58;
				}
			}

			hwnd = FindWindowExA( 0, 0, 0, "YAKA" ); //Find application window handle
			PostMessage( hwnd, WM_USER + 755, wParam, vkCode ); //Send info to app Window.
		}
	}
	return ( CallNextHookEx( hook, nCode, wParam, lParam ) ); //pass control to next hook in the hook chain.
}