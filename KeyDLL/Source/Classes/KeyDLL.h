#ifdef KEYDLL_EXPORTS
#define KEYDLL_API __declspec(dllexport)
#else
#define KEYDLL_API __declspec(dllimport)
#endif

//This function installs the Keyboard hook.
KEYDLL_API void installKeyHook( HWND h );

//This function removes the previously installed hook.
KEYDLL_API void removeKeyHook();

//This function is called when the keyboard is operated.
KEYDLL_API LRESULT CALLBACK hookProc( int ncode,
									  WPARAM wparam,
									  LPARAM lparam );
