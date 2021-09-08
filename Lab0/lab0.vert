#version 150

in  vec3  in_Position;
in  vec3  in_Normal;
in  vec2  in_TexCoord;
out vec3 exNormal; // Phong
out vec3 exSurface; // Phong (specular)

uniform mat4 projectionMatrix;
uniform mat4 modelToWorldToView;

void main(void)
{
	exNormal = inverse(transpose(mat3(modelToWorldToView))) * in_Normal; // Phong, normal transformation

	exSurface = vec3(modelToWorldToView * vec4(in_Position, 1.0)); // Don't include projection here - we only want to go to view coordinates

	gl_Position = projectionMatrix * modelToWorldToView * vec4(in_Position, 1.0); // This should include projection
}
