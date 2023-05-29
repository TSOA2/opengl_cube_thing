#version 330 core

in vec2 texture_position;
//in vec3 frag_position;
//in vec3 normal;

out vec4 FragColor;
uniform sampler2D texture_data;
//uniform vec3 light_position;

void main()
{
	FragColor = texture(texture_data, texture_position);
//	vec3 norm = normalize(normal);
//	vec3 direction = normalize(light_position - frag_position);
//	float diff = max(dot(norm, direction), 0.0);
//	vec3 diffuse = diff * vec3(0.5, 0.5, 1.0);

//	FragColor = vec4(diffuse * vec3(0.5, 0.5, 1.0), 1.0);
}
