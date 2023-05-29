#include "../include/common.h"

#include "../include/shaders.h"

#include "../include/vertices.h"

#include "../include/events.h"

#include "../include/entity.h"

GLFWwindow *init_gl()
{
    /*  
     * Initialize GLFW and set version
     */

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /*  
     * Last two parameters control monitor and window sharing,
     * so not needed
     */

    GLFWwindow *win = glfwCreateWindow(WIDTH, HEIGHT, WIN_NAME, NULL, NULL);
    if (win == NULL) {
        fprintf(stderr, "fatal: failed to create window.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }   

    /*  
     * Make window context current on calling thread. Only one
     * thread can have this at a time I guess
     */

    glfwMakeContextCurrent(win);

    /*  
     * Initialize GLAD, needed to manage function pointers
     */

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        fprintf(stderr, "fatal: failed to init glad.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }   

    /*  
     * First two parameters set location of lower left corner
     */

	glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(win, framebuffer_cb);
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetMouseButtonCallback(win, mouse_callback);
	return win;
}

int main(int argc, char **argv)
{
	GLFWwindow *win;

	Entity player;

	vbo obj_vbo;

	win = init_gl();

	obj_vbo = init_vbo(vertices, sizeof(vertices));

	player = init_entity(
		obj_vbo,
		"./images/container.jpg",
		V_SHADER_PATH,
		NULL,
		F_SHADER_PATH,
		(coord){2,0,0}
	);


	coord camera_pos = {0.0f, 0.0f, 3.0f};
	ray camera_vector = {0.0f, 0.0f, 0.0f};
	ray mouse_direction;

	mat4 view, projection;
	glm_mat4_identity(view);
	glm_mat4_identity(projection);

	glm_perspective(FOV, WIDTH/HEIGHT, 0.1f, 100.0f, projection);

	struct timeval start, stop;
	unsigned long diff;

	while (!glfwWindowShouldClose(win)) {
		gettimeofday(&start, NULL);

		/*
		 * Get mouse direction, process user events, set bg color
		 */

		get_mouse_direction(win, mouse_direction);
		process_events(win, camera_vector, mouse_direction);
		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*
		 * LOCAL SPACE -> MODEL MATRIX -> WORLD SPACE -> VIEW MATRIX ->
		 * VIEW SPACE -> PROJECTION MATRIX -> CLIP SPACE -> SCREEN SPACE
		 *
		 * LOCAL SPACE: the coordinates relative to the object
		 * WORLD SPACE: the coordinates relative to the world
		 * VIEW SPACE: coordinates as seen from camera's point of view
		 * CLIP SPACE: coordinates processed to -1.0 - 1.0 range, determines which vertices end up on screen
		 * SCREEN SPACE: coordinates as seen on screen
		 *
		 * Matrices are used to transform, scale, or rotate the vectors.
		 */


		/*
		 * Camera stuff
		 */

		vec3 tmp;
		glm_vec3_add(camera_pos, mouse_direction, tmp);

		glm_translate(view, camera_vector);
		glm_lookat(camera_pos, tmp, (vec3){0.0f, 1.0f, 0.0f}, view);

		/*
		 * Render "player" entity
		 */

		add_uniform_mat4(player.shader_prg, "view", view);
		add_uniform_mat4(player.shader_prg, "projection", projection);
//		add_uniform_vec3(player.shader_prg, "light_position", (coord) {0.0f, 0.0f, 0.0f});
		render_entity(player);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glm_vec3_copy(camera_vector, camera_pos);

		glfwPollEvents();
		glfwSwapBuffers(win);

		gettimeofday(&stop, NULL);
		diff = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
		fprintf(stderr, "fps: %lf\n", (double) 1000000 / diff);
	}

	destroy_entity(player);

	destroy_vbo(obj_vbo);

	fprintf(stderr, "exiting...\n");
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
