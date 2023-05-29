#include "../include/events.h"

static int events_locked = 0;

void mouse_callback(GLFWwindow *win, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		events_locked = 0;
	}
}

/* Get mouse direction */
void get_mouse_direction(GLFWwindow *win, vec3 dir)
{
	float xoffset, yoffset;
	double mouse_x, mouse_y;
	static double last_x, last_y;
	static int first_mouse;
	static double yaw, pitch;
	const float mouse_sensitivity = 0.05;

	if (!events_locked) {
		glfwGetCursorPos(win, &mouse_x, &mouse_y);
		if (!first_mouse) {
			last_x = mouse_x;
			last_y = mouse_y;
			first_mouse = 1;
		}

		xoffset = mouse_x - last_x;
		yoffset = last_y - mouse_y;
		last_x = mouse_x;
		last_y = mouse_y;

		xoffset *= mouse_sensitivity;
		yoffset *= mouse_sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f) {
			pitch = 89.0f;
		}

		if (pitch < -89.0f) {
			pitch = -89.0f;
		}

		dir[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
		dir[1] = sin(glm_rad(pitch));
		dir[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
	}
}

/* Process events */
void process_events(GLFWwindow *win, vec3 camera_pos, vec3 camera_dir)
{
	const float player_speed = 0.2;
	if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		events_locked = 1;
		return;
	}

	if (!events_locked) {
		if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) {
			vec3 tmp;
			glm_vec3_normalize_to(camera_dir, tmp);
			glm_vec3_scale(tmp, player_speed, tmp);
			glm_vec3_add(camera_pos, tmp, camera_pos);
			return;
		}

		if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
			vec3 tmp;
			glm_vec3_normalize_to(camera_dir, tmp);
			glm_vec3_scale(tmp, player_speed, tmp);
			glm_vec3_sub(camera_pos, tmp, camera_pos);
			return;
		}

		if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) {
			mat4 matrix;
			vec4 trans, end;
			vec3 final;

			glm_mat4_identity(matrix);
			glm_rotate(matrix, glm_rad(90), (vec3) {0.0f, 1.0f, 0.0f});

			glm_vec4(camera_dir, 1, trans);
			glm_mat4_mulv(matrix, trans, end);
			glm_vec4_copy3(end, final);

			glm_vec3_normalize(final);
			glm_vec3_scale(final, player_speed, final);

			camera_pos[0] += final[0];
			camera_pos[2] += final[2];
			return;
		}

		if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) {
			mat4 matrix;
			vec4 trans, end;
			vec3 final;

			glm_mat4_identity(matrix);
			glm_rotate(matrix, glm_rad(90), (vec3) {0.0f, -1.0f, 0.0f});
			
			glm_vec4(camera_dir, 1, trans);
			glm_mat4_mulv(matrix, trans, end);
			glm_vec4_copy3(end, final);

			glm_vec3_normalize(final);
			glm_vec3_scale(final, player_speed, final);

			camera_pos[0] += final[0];
			camera_pos[2] += final[2];
			return;
		}
	}

	return ;
}

/* Called when user resizes window */
void framebuffer_cb(GLFWwindow *win, int w, int h)
{
	/*
	 * NOTE: this does not resize the window.
	 */

	glViewport(0, 0, w, h);
}
