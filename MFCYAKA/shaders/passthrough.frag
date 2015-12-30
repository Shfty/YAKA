#version 400

in vec4 gl_FragCoord;
out vec4 gl_FragColor;

uniform ivec2 Resolution;

uniform sampler2D rTex;

void main()
{
	gl_FragColor = texture( rTex, vec2( gl_FragCoord.x / Resolution.x, gl_FragCoord.y / Resolution.y ) );
}