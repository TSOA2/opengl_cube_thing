#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "./cglm/cglm.h"

#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>

#define WIDTH (500)
#define HEIGHT (500)
#define WIN_NAME ("gl_try6")

#define FOV (45)

#define V_SHADER_PATH ("shaders/vertex.vert")
#define F_SHADER_PATH ("shaders/fragment.frag")
#define G_SHADER_PATH ("shaders/geometry.geom")

typedef unsigned int vbo;
typedef vec3 coord;
typedef vec3 ray;

#endif // COMMON_H
