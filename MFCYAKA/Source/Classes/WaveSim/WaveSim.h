#pragma once

#include <map>
#include <vector>
#include <algorithm>

#include "Core/GL.h"
#include "Core/KeyTimer.h"
#include "Core/Renderer.h"

namespace YAKA
{
	extern float viewport_points[];
	extern float quad_points[];

	class WaveSim : public Renderer
	{
	public:
		WaveSim();
		~WaveSim();

		virtual bool Init() override;
		virtual void Draw() override;
		virtual void KeyCallback( int key ) override;

		int main( GLFWwindow* window );

	private:
		bool setupVertexArrays();
		bool setupShaders();
		bool setupFramebuffers();
		bool setupRenderTextures();
		void drawKeys();
		void draw();
		void drawToKeyboard();
	};
}