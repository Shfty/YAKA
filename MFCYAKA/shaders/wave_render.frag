#version 400

in vec4 gl_FragCoord;
out vec4 gl_FragColor;

uniform ivec2 Resolution;
uniform vec3 BaseColour;
uniform vec3 RippleColour;
uniform float ColorMultiplier;

uniform sampler2D rTex;

void main()
{
	float alpha = texture( rTex, vec2( gl_FragCoord.x / Resolution.x, gl_FragCoord.y / Resolution.y ) ).r;
	gl_FragColor = vec4( mix( BaseColour, RippleColour * ColorMultiplier, alpha ), 1.0 );
}