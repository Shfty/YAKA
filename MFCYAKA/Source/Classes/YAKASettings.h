#pragma once

#include "stdafx.h"

#define TARGET_RESOLUTION_X				22
#define TARGET_RESOLUTION_Y				7

#define SUPERSAMPLE_FACTOR				4

#define INTERNAL_RESOLUTION_X			TARGET_RESOLUTION_X * SUPERSAMPLE_FACTOR
#define INTERNAL_RESOLUTION_Y			TARGET_RESOLUTION_Y * SUPERSAMPLE_FACTOR

#define WINDOW_RESOLUTION_X				880
#define WINDOW_RESOLUTION_Y				280

#define TARGET_FRAMERATE				120.0f
#define FRAME_TIME						( 1000.0f / TARGET_FRAMERATE )

namespace YAKA
{
	struct vec3
	{
		union
		{
			float x;
			float r;
		};
		union
		{
			float y;
			float g;
		};
		union
		{
			float z;
			float b;
		};

		vec3() : x( 0.0f ), y( 0.0f ), z( 0.0f )
		{
		}

		vec3( float x, float y, float z )
			: x( x ), y( y ), z( z )
		{
		}
	};

	class Config
	{
	public:
		static bool ShowVisualizer;
		static vec3 BaseColor;
		static vec3 RippleColor;
		static float RippleMultiplier;
		static float Damping;
		static float EdgeRebound;

		static void LoadConfig();

	private:
		static int intFromINI( LPCTSTR heading, LPCTSTR key, LPCTSTR path );
		static float floatFromINI( LPCTSTR heading, LPCTSTR key, LPCTSTR path );
		static vec3 vec3FromINI( LPCTSTR heading, LPCTSTR key, LPCTSTR path );
	};
}