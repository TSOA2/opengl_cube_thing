#version 330 core

in vec2 texture_position;

out vec4 FragColor;

uniform sampler2D texture_data;

void main()
{
	FragColor = texture(texture_data, texture_position);
}
