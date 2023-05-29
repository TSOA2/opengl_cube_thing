#ifndef EVENTS_H
#define EVENTS_H

#include "./common.h"

void mouse_callback(GLFWwindow *win, int button, int action, int mods);
void get_mouse_direction(GLFWwindow *win, vec3 dir);
void process_events(GLFWwindow *win, vec3 camera_pos, vec3 camera_dir);
void framebuffer_cb(GLFWwindow *win, int w, int h);

#endif // EVENTS_H
