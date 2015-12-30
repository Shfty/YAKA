#version 400

in vec4 gl_Vertex;

uniform ivec2 Offset;

void main()
{
	vec2 pixelSize = vec2( 2.0 / 22.0, 2.0 / 7.0 );
	vec2 halfPixelSize = pixelSize * 0.5;
	
	ivec2 ofs = Offset;
	ofs.x -= 10;
	
	ofs.y -= 3;
	ofs.y *= -1;
	
	gl_Position = vec4(gl_Vertex.x + (ofs.x * pixelSize.x), gl_Vertex.y + (ofs.y * pixelSize.y), 0.0, 1.0);
}