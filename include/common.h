#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>

#define WIDTH (500)
#define HEIGHT (500)
#define WIN_NAME ("gl_try2")

#define V_SHADER_PATH ("shaders/vertex.vert")
#define F_SHADER_PATH ("shaders/fragment.frag")
