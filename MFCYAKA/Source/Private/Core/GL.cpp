#include "stdafx.h"

#include "Core/GL.h"
using namespace YAKA;

bool GL::InitGL( GLFWwindow*& window )
{
	// start GL context and O/S window using the GLFW helper library
	if( !glfwInit() )
	{
		fprintf( stderr, "ERROR: could not start GLFW3\n" );
		return false;
	}

	if( !Config::ShowVisualizer )
	{
		glfwWindowHint( GLFW_VISIBLE, GL_FALSE );
	}

	window = glfwCreateWindow( WINDOW_RESOLUTION_X, WINDOW_RESOLUTION_Y, "Yet Another K70 App", NULL, NULL );
	if( !window )
	{
		fprintf( stderr, "ERROR: could not open window with GLFW3\n" );
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent( window );


	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString( GL_RENDERER ); // get renderer string
	const GLubyte* version = glGetString( GL_VERSION ); // version as a string
	printf( "Renderer: %s\n", renderer );
	printf( "OpenGL version supported %s\n", version );

	return true;
}

GLuint GL::LoadShaders( const GLchar* vertex_fname, const GLchar* fragment_fname )
{
	std::ifstream vert_in( vertex_fname );
	std::string vert_contents( ( std::istreambuf_iterator<char>( vert_in ) ),
							   std::istreambuf_iterator<char>() );
	const char* vert_shader = vert_contents.c_str();

	std::ifstream frag_in( fragment_fname );
	std::string frag_contents( ( std::istreambuf_iterator<char>( frag_in ) ),
							   std::istreambuf_iterator<char>() );
	const char* frag_shader = frag_contents.c_str();

	GLuint quad_vs = createAndCompileShader( GL_VERTEX_SHADER, &vert_shader );
	GLuint quad_fs = createAndCompileShader( GL_FRAGMENT_SHADER, &frag_shader );
	return createAndLinkProgram( quad_vs, quad_fs );
}

GLuint GL::createAndCompileShader( GLenum type, const GLchar* const* string )
{
	// create and compile shader
	GLuint id = glCreateShader( type );
	glShaderSource( id, 1, string, NULL );
	glCompileShader( id );

	// check for compile errors
	int params = -1;
	glGetShaderiv( id, GL_COMPILE_STATUS, &params );
	if( GL_TRUE != params )
	{
		fprintf( stderr, "ERROR: GL shader index %i did not compile\n", id );
		printShaderInfoLog( id );
		return false; // or exit or something
	}

	return id;
}

GLuint GL::createAndLinkProgram( GLuint vs, GLuint fs )
{
	GLuint shader_program = glCreateProgram();
	glAttachShader( shader_program, fs );
	glAttachShader( shader_program, vs );
	glLinkProgram( shader_program );

	// check if link was successful
	int params = -1;
	glGetProgramiv( shader_program, GL_LINK_STATUS, &params );
	if( GL_TRUE != params )
	{
		fprintf(
			stderr,
			"ERROR: could not link shader programme GL index %u\n",
			shader_program
			);
		printProgramInfoLog( shader_program );
		return -1;
	}

	return shader_program;
}

GLuint GL::CreateVertexArray( GLuint& vbo, float* vertices, int numFloats )
{
	createVertexBuffer( vbo, vertices, numFloats );

	// generate and setup vertex array object
	GLuint vao = 0;
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
	glEnableVertexAttribArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );

	return vao;
}

void GL::createVertexBuffer( GLuint& vbo, float* vertices, int numFloats )
{
	// generate and populate vertex buffer object
	glGenBuffers( 1, &vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, numFloats * sizeof( GLfloat ), vertices, GL_STATIC_DRAW );
}

void GL::printShaderInfoLog( GLuint shader_index )
{
	int max_length = 2048;
	int actual_length = 0;
	char log[2048];
	glGetShaderInfoLog( shader_index, max_length, &actual_length, log );
	printf( "shader info log for GL index %u:\n%s\n", shader_index, log );
}

void GL::printProgramInfoLog( GLuint program )
{
	int max_length = 2048;
	int actual_length = 0;
	char log[2048];
	glGetProgramInfoLog( program, max_length, &actual_length, log );
	printf( "program info log for GL index %u:\n%s", program, log );
}