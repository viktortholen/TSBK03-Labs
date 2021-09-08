#version 150
//phong with one light source without ambient light

in vec3 exNormal;
in vec3 exSurface; //specular

out vec4 out_Color;

void main(void)
{
	// float ambientStrength = 0.1;
	// vec3 ambient = ambientStrength * lightColor;
	// vec3 result = ambient * objectColor;
	// out_Color=vec4(result,1.0);

	const vec3 light = vec3(0.58, 0.58, 0.58);
	float diffuse, specular, shade;

	//Diffuse
	diffuse = dot(normalize(exNormal), light);
	diffuse = max(0.0, diffuse); //no negative light

	//specular
	vec3 r = reflect(-light, normalize(exNormal));
	vec3 v = normalize(-exSurface); //view direction
	specular = dot(r,v);
	if(specular > 0.0)
		specular = 1.0 * pow(specular, 150.0);
	specular = max(specular, 0.0);
	shade = 0.7*diffuse + 1.0 * specular;
	
	out_Color = vec4(shade,shade,shade, 1.0);
}

