#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex_pos;

out vec2 texture_position;
//out vec3 frag_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
//	frag_position = vec3(model * vec4(pos, 1.0));
	texture_position = tex_pos;
}
