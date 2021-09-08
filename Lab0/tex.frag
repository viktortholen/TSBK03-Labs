#version 150

out vec4 out_Color;
in vec3 exNormal;
in vec2 exTexCoord;

void main(void)
{
// Anything that uses the texture coordinates!
	out_Color = vec4(exTexCoord.s, exTexCoord.t, 1.0, 1.0);
}