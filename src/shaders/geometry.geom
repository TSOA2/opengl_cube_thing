#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out vec3 normal;

void main()
{
	vec3 a = gl_in[0].gl_Position.xyz - gl_in[1].gl_Position.xyz;
	vec3 b = gl_in[2].gl_Position.xyz - gl_in[1].gl_Position.xyz;
	normal = cross(a, b);

	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	gl_Position = gl_in[2].gl_Position;
	EmitVertex();

	EndPrimitive();
}
