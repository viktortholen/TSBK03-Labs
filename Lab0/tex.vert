#version 150

in  vec3  in_Position;
in  vec3  in_Normal;
in  vec2  in_TexCoord;
out vec3 exNormal;
out vec2 exTexCoord;

uniform mat4 projectionMatrix;
uniform mat4 modelToWorldToView;

void main(void)
{
	exNormal = inverse(transpose(mat3(modelToWorldToView))) * in_Normal; // Phong, "fake" normal transformation

	exTexCoord = in_TexCoord;

	gl_Position = projectionMatrix * modelToWorldToView * vec4(in_Position, 1.0); // This should include projection
}