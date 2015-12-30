#pragma once

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

#include <iostream>
#include <fstream>

namespace YAKA
{
	class GL
	{
	public:
		static bool InitGL( GLFWwindow*& window );
		static GLuint LoadShaders( const GLchar* vertex_string, const GLchar* fragment_string );
		static GLuint CreateVertexArray( GLuint& vbo, float* vertices, int numFloats );

	private:
		static GLuint createAndCompileShader( GLenum type, const GLchar* const* string );
		static GLuint createAndLinkProgram( GLuint vs, GLuint fs );

		static void createVertexBuffer( GLuint& vbo, float* vertices, int numFloats );

		static void printShaderInfoLog( GLuint shader_index );
		static void printProgramInfoLog( GLuint program );
	};
}