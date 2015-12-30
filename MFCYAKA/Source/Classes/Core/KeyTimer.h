#pragma once

namespace YAKA
{
	class KeyTimer
	{
	public:
		int x;
		int y;

		float duration;
		float timer;

		KeyTimer( int x, int y );
		KeyTimer( int x, int y, float duration );
		~KeyTimer();

		void Update( float deltaTime );
	};
}