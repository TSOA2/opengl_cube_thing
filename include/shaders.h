#pragma once

char *get_src(const char *);
int compile_shader(const char *, int);
void destroy_shader(int);
int crt_shader_program(int *, int);
void destroy_shader_program(int);
