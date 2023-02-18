#include "../include/common.h"
#include "../include/shaders.h"

/* Get source of file */
char *get_src(const char *fname)
{
    struct stat s;
    int fd = open(fname, O_RDONLY);
    if (fd < 0) {
        return NULL;
    }

    if (fstat(fd, &s) < 0) {
        return NULL;
    }

    char *buf = malloc(s.st_size + 1);
    if (buf == NULL) {
        return NULL;
    }

    if (read(fd, buf, s.st_size) != s.st_size) {
        return NULL;
    }

    buf[s.st_size] = '\0';
    close(fd);
    return buf;
}

/* Compile a shader given a filename and type */
int compile_shader(const char *fname, int type)
{
	int shader = glCreateShader(type);
	int success = 1;
	char error_msg[512];
	char *shader_txt = get_src(fname);
	if (shader_txt == NULL) {
		fprintf(stderr, "fatal: could not read shader file \'%s\'\n", fname);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glShaderSource(shader, 1, (const char * const *) &shader_txt, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, sizeof(error_msg), NULL, error_msg);
		fprintf(stderr, "fatal: failed to compile shader file \'%s\'\n", error_msg);
		free(shader_txt);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	free(shader_txt);
	return shader;
}

/* Destroy a shader - not needed, but looks nicer */
void destroy_shader(int shader)
{
	glDeleteShader(shader);
}

/* Create a program out of the shaders */
int crt_shader_program(int *shaders, int shader_len)
{
	int success = 1;
	int program = glCreateProgram();
	char error_msg[512];

	for (int i = 0; i < shader_len; i++) {
		glAttachShader(program, shaders[i]);
	}

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, sizeof(error_msg), NULL, error_msg);
		fprintf(stderr, "fatal: failed to link shaders: \'%s\'\n", error_msg);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	return program;
}

/* Destroy a shader program - not needed, but looks nicer */
void destroy_shader_program(int shader_prg)
{
	glDeleteProgram(shader_prg);
}

