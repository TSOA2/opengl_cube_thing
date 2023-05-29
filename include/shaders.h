#ifndef SHADERS_H
#define SHADERS_H


int compile_shader(const char *, int);
void destroy_shader(int);
int crt_shader_program(int *, int);
void destroy_shader_program(int);

void add_uniform_mat4(int shader, const char *name, mat4 m);
void add_uniform_vec3(int shader, const char *name, vec3 v);
#endif // SHADERS_H
