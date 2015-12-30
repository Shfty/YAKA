#version 400

in vec4 gl_FragCoord;
out vec4 gl_FragColor;

uniform ivec2 Resolution;
uniform float Damping;
uniform float EdgeRebound;

uniform sampler2D Buffer1;
uniform sampler2D Buffer2;

void main()
{
	vec2 pixelSize = vec2(1,1) / Resolution.xy;
	vec2 uv = vec2( gl_FragCoord.x / Resolution.x, gl_FragCoord.y / Resolution.y );
	
	float xMin = clamp(uv.x - pixelSize.x, 0.0, 1.0);
	float xMax = clamp(uv.x + pixelSize.x, 0.0, 1.0);
	float yMin = clamp(uv.y - pixelSize.y, 0.0, 1.0);
	float yMax = clamp(uv.y + pixelSize.y, 0.0, 1.0);
	
	float color = (texture( Buffer1, vec2( uv.x - pixelSize.x, uv.y ) ).r +
			 texture( Buffer1, vec2( uv.x + pixelSize.x, uv.y ) ).r +
			 texture( Buffer1, vec2( uv.x, uv.y + pixelSize.y ) ).r +
			 texture( Buffer1, vec2( uv.x, uv.y - pixelSize.y ) ).r ) * 0.5 - texture( Buffer2, vec2( uv.x, uv.y ) ).r;
	color *= Damping;
	
	if( uv.x <= pixelSize.x || uv.y <= pixelSize.y || uv.x >= 1.0 - pixelSize.x || uv.y >= 1.0 - pixelSize.y )
	{
		color *= EdgeRebound;
	}
	
	gl_FragColor = vec4( color, 0.0, 0.0, 1.0 );
}