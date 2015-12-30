#include "stdafx.h"

#include "WaveSim/WaveSim.h"
using namespace YAKA;

float YAKA::viewport_points[] = {
	-1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f
};

float pX = 1.0f / float( TARGET_RESOLUTION_X );
float pY = 1.0f / float( TARGET_RESOLUTION_Y );
float hpX = pX * 0.5f;
float hpY = pY * 0.5f;

float YAKA::quad_points[] = {
	-hpX - pX,  hpY, 0.0f,
	 hpX - pX,  hpY, 0.0f,
	 hpX - pX, -hpY, 0.0f,
	-hpX - pX, -hpY, 0.0f
};

GLuint wave_compute_shader_program = -1;
GLuint quad_shader_program = -1;
GLuint wave_render_shader_program = -1;
GLuint upscale_shader_program = -1;

GLuint wave_compute_vao = 0;
GLuint quad_vao = 0;


unique_ptr<GLuint[]> compute_rtex_names ( new GLuint[2] );
GLuint render_rtex_name = -1;

unique_ptr<GLuint[]> compute_fbo_names( new GLuint[2] );
GLuint render_fbo_name = -1;

unique_ptr<GLuint[]> wave_compute_rtex_ids( new GLuint[2] );
GLuint wave_compute_resolution_id = -1;
GLuint wave_compute_damping_id = -1;
GLuint wave_compute_edgerebound_id = -1;
unique_ptr<GLuint[]> wave_compute_sampler_names( new GLuint[2] );

GLuint quad_offset_id = -1;

GLuint wave_render_rtex_id = -1;
GLuint wave_render_sampler_name = -1;
GLuint wave_render_resolution_id = -1;
GLuint wave_render_basecolour_id = -1;
GLuint wave_render_ripplecolour_id = -1;
GLuint wave_render_colourmult_id = -1;

GLuint upscale_rtex_id = -1;
GLuint upscale_sampler_name = -1;
GLuint upscale_resolution_id = -1;

std::map<int, int> vkey_corsair_mapping = {
	{ VK_ESCAPE, CLK_Escape },
	{ VK_F1, CLK_F1 },
	{ VK_F2, CLK_F2 },
	{ VK_F3, CLK_F3 },
	{ VK_F4, CLK_F4 },
	{ VK_F5, CLK_F5 },
	{ VK_F6, CLK_F6 },
	{ VK_F7, CLK_F7 },
	{ VK_F8, CLK_F8 },
	{ VK_F9, CLK_F9 },
	{ VK_F10, CLK_F10 },
	{ VK_F11, CLK_F11 },
	{ VK_OEM_8, CLK_GraveAccentAndTilde },
	{ '1', CLK_1 },
	{ '2', CLK_2 },
	{ '3', CLK_3 },
	{ '4', CLK_4 },
	{ '5', CLK_5 },
	{ '6', CLK_6 },
	{ '7', CLK_7 },
	{ '8', CLK_8 },
	{ '9', CLK_9 },
	{ '0', CLK_0 },
	{ VK_OEM_MINUS, CLK_MinusAndUnderscore },
	{ VK_TAB, CLK_Tab },
	{ 'Q', CLK_Q },
	{ 'W', CLK_W },
	{ 'E', CLK_E },
	{ 'R', CLK_R },
	{ 'T', CLK_T },
	{ 'Y', CLK_Y },
	{ 'U', CLK_U },
	{ 'I', CLK_I },
	{ 'O', CLK_O },
	{ 'P', CLK_P },
	{ VK_OEM_4, CLK_BracketLeft },
	{ VK_CAPITAL, CLK_CapsLock },
	{ VK_CAPITAL, CLK_CapsLock },
	{ 'A', CLK_A },
	{ 'S', CLK_S },
	{ 'D', CLK_D },
	{ 'F', CLK_F },
	{ 'G', CLK_G },
	{ 'H', CLK_H },
	{ 'J', CLK_J },
	{ 'K', CLK_K },
	{ 'L', CLK_L },
	{ VK_OEM_1, CLK_SemicolonAndColon },
	{ VK_OEM_3, CLK_ApostropheAndDoubleQuote },
	{ VK_OEM_7, CLK_NonUsTilde },
	{ VK_LSHIFT, CLK_LeftShift },
	{ VK_OEM_5, CLK_NonUsBackslash },
	{ 'Z', CLK_Z },
	{ 'X', CLK_X },
	{ 'C', CLK_C },
	{ 'V', CLK_V },
	{ 'B', CLK_B },
	{ 'N', CLK_N },
	{ 'M', CLK_M },
	{ VK_OEM_COMMA, CLK_CommaAndLessThan },
	{ VK_OEM_PERIOD, CLK_PeriodAndBiggerThan },
	{ VK_OEM_2, CLK_SlashAndQuestionMark },
	{ VK_LCONTROL, CLK_LeftCtrl },
	{ VK_LWIN, CLK_LeftGui },
	{ VK_LMENU, CLK_LeftAlt },
	{ VK_SPACE, CLK_Space },
	{ VK_RMENU, CLK_RightAlt },
	{ VK_RWIN, CLK_RightGui },
	{ VK_APPS, CLK_Application },
	{ VK_F12, CLK_F12 },
	{ VK_SNAPSHOT, CLK_PrintScreen },
	{ VK_SCROLL, CLK_ScrollLock },
	{ VK_PAUSE, CLK_PauseBreak },
	{ VK_INSERT, CLK_Insert },
	{ VK_HOME, CLK_Home },
	{ VK_PRIOR, CLK_PageUp },
	{ VK_OEM_6, CLK_BracketRight },
	{ VK_RETURN, CLK_Enter },
	{ VK_OEM_PLUS, CLK_EqualsAndPlus },
	{ VK_BACK, CLK_Backspace },
	{ VK_DELETE, CLK_Delete },
	{ VK_END, CLK_End },
	{ VK_NEXT, CLK_PageDown },
	{ VK_RSHIFT, CLK_RightShift },
	{ VK_RCONTROL, CLK_RightCtrl },
	{ VK_UP, CLK_UpArrow },
	{ VK_LEFT, CLK_LeftArrow },
	{ VK_DOWN, CLK_DownArrow },
	{ VK_RIGHT, CLK_RightArrow },
	{ VK_VOLUME_MUTE, CLK_Mute },
	{ VK_MEDIA_STOP, CLK_Stop },
	{ VK_MEDIA_PREV_TRACK, CLK_ScanPreviousTrack },
	{ VK_MEDIA_PLAY_PAUSE, CLK_PlayPause },
	{ VK_MEDIA_NEXT_TRACK, CLK_ScanNextTrack },
	{ VK_NUMLOCK, CLK_NumLock },
	{ VK_NUMPAD7, CLK_Keypad7 },
	{ VK_NUMPAD8, CLK_Keypad8 },
	{ VK_NUMPAD9, CLK_Keypad9 },
	{ VK_NUMPAD4, CLK_Keypad4 },
	{ VK_NUMPAD5, CLK_Keypad5 },
	{ VK_NUMPAD6, CLK_Keypad6 },
	{ VK_NUMPAD1, CLK_Keypad1 },
	{ VK_NUMPAD2, CLK_Keypad2 },
	{ VK_NUMPAD3, CLK_Keypad3 },
	{ VK_NUMPAD0, CLK_Keypad0 },
	{ VK_DIVIDE, CLK_KeypadSlash },
	{ VK_MULTIPLY, CLK_KeypadAsterisk },
	{ VK_SUBTRACT, CLK_KeypadMinus },
	{ VK_ADD, CLK_KeypadPlus },
	{ VK_DECIMAL, CLK_KeypadPeriodAndDelete },
	{ 58, CLK_KeypadEnter },
};

CorsairLedPositions* ledPositions = nullptr; // Raw pointer, managed by Corsair SDK
unique_ptr<CorsairLedColor[]> ledColors = nullptr;

std::vector<KeyTimer*> keyTimers;

WaveSim::WaveSim()
{

}

WaveSim::~WaveSim()
{

}

bool WaveSim::Init()
{
	/*
	* Init OpenGL
	*/
	if( !setupVertexArrays() )
	{
		return false;
	}

	if( !setupShaders() )
	{
		return false;
	}

	if( !setupRenderTextures() )
	{
		return false;
	}

	if( !setupFramebuffers() )
	{
		return false;
	}

	glClear( GL_COLOR_BUFFER_BIT );

	/*
	* Init Corsair SDK
	*/
	ledPositions = CorsairGetLedPositions();
	ledColors.reset( new CorsairLedColor[ledPositions->numberOfLed] );

	return true;
}

void WaveSim::Draw()
{
	drawKeys();
	draw();
	drawToKeyboard();
}

bool WaveSim::setupVertexArrays()
{
	GL::CreateVertexArray( wave_compute_vao, viewport_points, sizeof( viewport_points ) );
	GL::CreateVertexArray( quad_vao, quad_points, sizeof( quad_points ) );

	if( wave_compute_vao == -1 || quad_vao == -1 )
	{
		return false;
	}

	return true;
}

bool WaveSim::setupShaders()
{
	// Wave compute shader
	wave_compute_shader_program = GL::LoadShaders( "shaders\\basic.vert", "shaders\\wave_compute.frag" );
	wave_compute_rtex_ids[0] = glGetUniformLocation( wave_compute_shader_program, "Buffer1" );
	wave_compute_rtex_ids[1] = glGetUniformLocation( wave_compute_shader_program, "Buffer2" );
	glGenSamplers( 2, wave_compute_sampler_names.get() );
	glSamplerParameteri( wave_compute_sampler_names[0], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glSamplerParameteri( wave_compute_sampler_names[0], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glSamplerParameteri( wave_compute_sampler_names[0], GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glSamplerParameteri( wave_compute_sampler_names[0], GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glSamplerParameteri( wave_compute_sampler_names[1], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glSamplerParameteri( wave_compute_sampler_names[1], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glSamplerParameteri( wave_compute_sampler_names[1], GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glSamplerParameteri( wave_compute_sampler_names[1], GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	wave_compute_resolution_id = glGetUniformLocation( wave_compute_shader_program, "Resolution" );
	wave_compute_damping_id = glGetUniformLocation( wave_compute_shader_program, "Damping" );
	wave_compute_edgerebound_id = glGetUniformLocation( wave_compute_shader_program, "EdgeRebound" );

	// Debug colour shader
	quad_shader_program = GL::LoadShaders( "shaders\\key_position.vert", "shaders\\debug_block.frag" );
	quad_offset_id = glGetUniformLocation( quad_shader_program, "Offset" );

	// Wave render shader
	wave_render_shader_program = GL::LoadShaders( "shaders\\basic.vert", "shaders\\wave_render.frag" );
	wave_render_rtex_id = glGetUniformLocation( wave_render_shader_program, "rTex" );
	glGenSamplers( 1, &wave_render_sampler_name );
	glSamplerParameteri( wave_render_sampler_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glSamplerParameteri( wave_render_sampler_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	wave_render_resolution_id = glGetUniformLocation( wave_render_shader_program, "Resolution" );
	wave_render_basecolour_id = glGetUniformLocation( wave_render_shader_program, "BaseColour" );
	wave_render_ripplecolour_id = glGetUniformLocation ( wave_render_shader_program, "RippleColour" );
	wave_render_colourmult_id = glGetUniformLocation ( wave_render_shader_program, "ColorMultiplier" );

	// Upscale shader
	upscale_shader_program = GL::LoadShaders( "shaders\\basic.vert", "shaders\\passthrough.frag" );
	upscale_rtex_id = glGetUniformLocation( wave_render_shader_program, "rTex" );
	glGenSamplers( 1, &upscale_sampler_name );
	glSamplerParameteri( upscale_sampler_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glSamplerParameteri( upscale_sampler_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	upscale_resolution_id = glGetUniformLocation( upscale_shader_program, "Resolution" );

	if( wave_compute_shader_program == -1 || quad_shader_program == -1 || wave_render_shader_program == -1 )
	{
		return false;
	}

	return true;
}

bool WaveSim::setupRenderTextures()
{
	// wave compute textures
	glGenTextures( 2, compute_rtex_names.get() );

	glBindTexture( GL_TEXTURE_2D, compute_rtex_names[0] );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, INTERNAL_RESOLUTION_X, INTERNAL_RESOLUTION_Y, 0, GL_RED, GL_FLOAT, NULL );

	glBindTexture( GL_TEXTURE_2D, compute_rtex_names[1] );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, INTERNAL_RESOLUTION_X, INTERNAL_RESOLUTION_Y, 0, GL_RED, GL_FLOAT, NULL );

	// wave render texture
	glGenTextures( 1, &render_rtex_name );

	glBindTexture( GL_TEXTURE_2D, render_rtex_name );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, TARGET_RESOLUTION_X, TARGET_RESOLUTION_Y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );


	// TODO: Add error checking

	return true;
}

bool WaveSim::setupFramebuffers()
{
	glGenFramebuffers( 2, compute_fbo_names.get() );

	glBindFramebuffer( GL_FRAMEBUFFER, compute_fbo_names[0] );
	glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, compute_rtex_names[0], 0 );

	glBindFramebuffer( GL_FRAMEBUFFER, compute_fbo_names[1] );
	glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, compute_rtex_names[1], 0 );

	glGenFramebuffers( 1, &render_fbo_name );

	glBindFramebuffer( GL_FRAMEBUFFER, render_fbo_name );
	glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, render_rtex_name, 0 );

	if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
	{
		return false;
	}

	return true;
}

GLuint front_buffer_idx = 0;
GLuint back_buffer_idx = 1;
void WaveSim::drawKeys()
{
	for( unsigned int i = 0; i < keyTimers.size(); ++i )
	{
		KeyTimer* keyTimer = keyTimers[i];

		keyTimer->Update( FRAME_TIME );

		if( keyTimer->timer >= keyTimer->duration )
		{
			keyTimers.erase( keyTimers.begin() + i );
			--i;
			continue;
		}

		glBindFramebuffer( GL_FRAMEBUFFER, compute_fbo_names[front_buffer_idx] );

		glViewport( 0, 0, INTERNAL_RESOLUTION_X, INTERNAL_RESOLUTION_Y );
		glUseProgram( quad_shader_program );
		glUniform2i( quad_offset_id, keyTimer->x, keyTimer->y );

		glBindVertexArray( quad_vao );
		glDrawArrays( GL_QUADS, 0, 4 );
	}
}

void WaveSim::draw()
{
	/**
	 * Bind the render texture framebuffer
	 */
	glBindFramebuffer( GL_FRAMEBUFFER, compute_fbo_names[back_buffer_idx] );

	glViewport( 0, 0, INTERNAL_RESOLUTION_X, INTERNAL_RESOLUTION_Y );

	/**
	 * Wave compute shader
	 */
	glUseProgram( wave_compute_shader_program );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, compute_rtex_names[front_buffer_idx] );
	glUniform1i( wave_compute_rtex_ids[front_buffer_idx], front_buffer_idx );
	glBindSampler( 0, wave_compute_sampler_names[front_buffer_idx] );

	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_2D, compute_rtex_names[back_buffer_idx] );
	glUniform1i( wave_compute_rtex_ids[back_buffer_idx], back_buffer_idx );
	glBindSampler( 0, wave_compute_sampler_names[back_buffer_idx] );

	glUniform2i( wave_compute_resolution_id, INTERNAL_RESOLUTION_X, INTERNAL_RESOLUTION_Y );
	glUniform1f( wave_compute_damping_id, Config::Damping );
	glUniform1f( wave_compute_edgerebound_id, Config::EdgeRebound );

	glBindVertexArray( wave_compute_vao );
	// draw points 0-4 from the currently bound VAO with current in-use shader
	glDrawArrays( GL_QUADS, 0, 4 );

	/**
	* Bind the upscale render texture framebuffer
	*/
	glBindFramebuffer( GL_FRAMEBUFFER, render_fbo_name );
	glViewport( 0, 0, TARGET_RESOLUTION_X, TARGET_RESOLUTION_Y );

	/**
	* Wave render shader
	*/
	glUseProgram( wave_render_shader_program );
	glUniform1i( wave_render_rtex_id, 0 );
	glBindSampler( 0, wave_render_sampler_name );

	glUniform2i( wave_render_resolution_id, TARGET_RESOLUTION_X, TARGET_RESOLUTION_Y );
	glUniform3f( wave_render_basecolour_id, Config::BaseColor.r, Config::BaseColor.g, Config::BaseColor.b );
	glUniform3f ( wave_render_ripplecolour_id, Config::RippleColor.r, Config::RippleColor.g, Config::RippleColor.b );
	glUniform1f ( wave_render_colourmult_id, Config::RippleMultiplier );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, compute_rtex_names[back_buffer_idx] );

	glBindVertexArray( wave_compute_vao );
	// draw points 0-4 from the currently bound VAO with current in-use shader
	glDrawArrays( GL_QUADS, 0, 4 );

	/**
	* Bind the window framebuffer
	*/
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glViewport( 0, 0, WINDOW_RESOLUTION_X, WINDOW_RESOLUTION_Y );

	/**
	* Upscale shader
	*/

	// wipe the drawing surface clear
	glClear( GL_COLOR_BUFFER_BIT );

	glUseProgram( upscale_shader_program );
	glUniform1i(upscale_rtex_id, 0 );
	glBindSampler( 0, upscale_sampler_name );

	glUniform2i( upscale_resolution_id, WINDOW_RESOLUTION_X, WINDOW_RESOLUTION_Y );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, render_rtex_name );

	glBindVertexArray( wave_compute_vao );
	// draw points 0-4 from the currently bound VAO with current in-use shader
	glDrawArrays( GL_QUADS, 0, 4 );

	/**
	* Swap buffers
	*/
	front_buffer_idx = ++front_buffer_idx % 2;
	back_buffer_idx = ++back_buffer_idx % 2;
}

#define CORSAIR_WIDTH	433.0
#define CORSAIR_HEIGHT	138.0
void WaveSim::drawToKeyboard()
{
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, render_rtex_name );

	float pixels[TARGET_RESOLUTION_X * TARGET_RESOLUTION_Y * 3];
	glGetTexImage( GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, pixels );

	if( ledPositions != nullptr )
	{
		for( int i = 0; i < ledPositions->numberOfLed; ++i )
		{
			CorsairLedPosition led = ledPositions->pLedPosition[i];

			double ledCenterX = led.left + led.width * 0.5f;
			double ledCenterY = led.top + led.height * 0.5f;

			int x = (int)( ( ledCenterX / CORSAIR_WIDTH ) * TARGET_RESOLUTION_X );
			int y = (int)( ( ledCenterY / CORSAIR_HEIGHT ) * TARGET_RESOLUTION_Y );

			int index = ( ( TARGET_RESOLUTION_Y - y - 1 ) * TARGET_RESOLUTION_X + x ) * 3;
			float r = pixels[index + 0];
			float g = pixels[index + 1];
			float b = pixels[index + 2];

			ledColors[i] = CorsairLedColor{ led.ledId, int( r * 255.0f ), int( g * 255.0f ), int( b * 255.0f ) };
		}

		CorsairSetLedsColorsAsync( ledPositions->numberOfLed, ledColors.get(), nullptr, nullptr );
	}
}

void WaveSim::KeyCallback( int key )
{
	if( ledPositions != nullptr )
	{
		// If the key mapping contains the pressed key
		if( vkey_corsair_mapping.find( key ) != vkey_corsair_mapping.end() )
		{
			// Store the mapped key
			int corsairKey = vkey_corsair_mapping.at( key );

			// Iterate through all the corsair LED positions
			for( int i = 0; i < ledPositions->numberOfLed; ++i )
			{
				// If the current LED position's ID is equal to the maped corsair key
				if( ledPositions->pLedPosition[i].ledId == corsairKey )
				{
					// Get the LED position data
					CorsairLedPosition led = ledPositions->pLedPosition[i];

					// Calculate the LED's X/Y position
					double ledCenterX = led.left + led.width * 0.5f;
					double ledCenterY = led.top + led.height * 0.5f;

					int x = (int)( ( ledCenterX / CORSAIR_WIDTH ) * TARGET_RESOLUTION_X );
					int y = (int)( ( ledCenterY / CORSAIR_HEIGHT ) * TARGET_RESOLUTION_Y );

					// Create a new KeyTimer at that position
					KeyTimer* timer = new KeyTimer( x, y );
					keyTimers.push_back( timer );
				}
			}
		}
	}
}