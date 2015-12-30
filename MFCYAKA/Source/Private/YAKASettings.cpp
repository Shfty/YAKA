#include "stdafx.h"
using namespace YAKA;

bool Config::ShowVisualizer = false;
vec3 Config::BaseColor = vec3( 0.0f, 0.0f, 0.1f );
vec3 Config::RippleColor = vec3( 0.0f, 1.0f, 1.0f );
float Config::RippleMultiplier = 2.0f;
float Config::Damping = 1.0f;
float Config::EdgeRebound = 0.0f;

void Config::LoadConfig()
{
	LPCTSTR path = L".\\YAKA.ini";

	ShowVisualizer = intFromINI( _T( "Core" ), _T( "ShowVisualizer" ), path ) == 0 ? false : true;
	BaseColor = vec3FromINI( _T( "WaveSim" ), _T( "BaseColor" ), path );
	RippleColor = vec3FromINI( _T( "WaveSim" ), _T( "RippleColor" ), path );
	RippleMultiplier = floatFromINI( _T( "WaveSim" ), _T( "RippleMultiplier" ), path );
	Damping = floatFromINI( _T( "WaveSim" ), _T( "Damping" ), path );
	EdgeRebound = floatFromINI( _T( "WaveSim" ), _T( "EdgeRebound" ), path );
}

int Config::intFromINI( LPCTSTR heading, LPCTSTR key, LPCTSTR path )
{
	return GetPrivateProfileInt( heading, key, 0, path );
}

float Config::floatFromINI( LPCTSTR heading, LPCTSTR key, LPCTSTR path )
{
	TCHAR* next_token = NULL;
	TCHAR  charBuffer[256];

	GetPrivateProfileString( heading, key, _T( "" ), charBuffer, sizeof( charBuffer ), path );
	return _tcstof( charBuffer, &next_token );
}

vec3 Config::vec3FromINI( LPCTSTR heading, LPCTSTR key, LPCTSTR path )
{
	TCHAR* next_tokenA = NULL;
	TCHAR* next_tokenB = NULL;
	TCHAR  charBuffer[256];

	GetPrivateProfileString( heading, key, _T( "" ), charBuffer, sizeof( charBuffer ), path );

	vec3 vec;

	vec.x = _tcstof( _tcstok_s( charBuffer, _T( ", " ), &next_tokenA ), &next_tokenB );
	vec.y = _tcstof( _tcstok_s( NULL, _T( ", " ), &next_tokenA ), &next_tokenB );
	vec.z = _tcstof( _tcstok_s( NULL, _T( ", " ), &next_tokenA ), &next_tokenB );

	return vec;
}