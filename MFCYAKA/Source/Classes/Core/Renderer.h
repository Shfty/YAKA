#pragma once

namespace YAKA
{
	class Renderer
	{
	public:
		virtual bool Init() { return true; };
		virtual void Draw() {};
		virtual void KeyCallback( int key ) {};
	};
}