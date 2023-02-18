#include "../include/common.h"
#include "../include/shaders.h"
#include "../include/cglm/cglm.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stbi.h"


/* Get mouse direction */
void get_mouse_direction(GLFWwindow *win, vec3 dir)
{
	double mouse_x, mouse_y;
	glfwGetCursorPos(win, &mouse_x, &mouse_y);

	static double last_x, last_y;
	static int first_mouse;
	static double yaw, pitch;

	if (!first_mouse) {
		last_x = mouse_x;
		last_y = mouse_y;
		first_mouse = 1;
	}

	float xoffset = mouse_x - last_x;
	float yoffset = last_y - mouse_y;
	last_x = mouse_x;
	last_y = mouse_y;

	const float mouse_sensitivity = 0.05f;
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

/* Process events */
void process_events(GLFWwindow *win, vec3 camera_pos, vec3 camera_dir)
{
	const float player_speed = 0.2;
	if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(win, 1);
		return;
	}

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

/* Called when user resizes window */
void framebuffer_cb(GLFWwindow *win, int w, int h)
{
	/*
	 * NOTE: this does not resize the window.
	 */

	glViewport(0, 0, w, h);
}


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
	return win;
}

int main(int argc, char **argv)
{
	GLFWwindow *win = init_gl();

	int program;
	int v_shader, f_shader;
	int shaders[2];

	v_shader = compile_shader(V_SHADER_PATH, GL_VERTEX_SHADER);
	f_shader = compile_shader(F_SHADER_PATH, GL_FRAGMENT_SHADER);
	shaders[0] = v_shader;
	shaders[1] = f_shader;
	program = crt_shader_program(shaders, 2);

	/*
	 * Vertices and objects, VBO & VAO stuff, yeah
	 */

	/*
	 * CUBE!
	 */

	float vertices[] = {
    	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

//	unsigned int indices[] = {
//		0, 1, 2,
//		2, 3, 0
//	};

	unsigned int vbo, vao;
//	unsigned int ebo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
//	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/*
	 * ______________________________________________________________________
	 * |      VERTEX 1       |        VERTEX 2       |      VERTEX 3        |
	 * ----------------------------------------------------------------------
	 * | X | Y | Z | TX | TY ||  X | Y | Z | TX | TY || X | Y | Z | TX | TY |
	 * ----------------------------------------------------------------------
	 * | POSITION  | TEX POS || POSITION   | TEX POS || POSITION  | TEX POS |
	 * ----------------------------------------------------------------------
	 *
	 * Each "container" here takes up 4 bytes in the GPU's memory.
	 *
	 * The first two arguments of glVertexAttribPointer are the index and the size.
	 * 1. The INDEX is which set of elements it is. For example, index zero is the
	 * 		POSITION field (x, y, and z). Additionally, index one is the start of the
	 * 		TEX POS field (the texture position).
	 * 2. The SIZE is the number of "container"s the field takes up. For example, the
	 * 		POSITION field takes up 3 containers, whereas the TEX POS field takes up
	 * 		2 containers.
	 * 
	 * The third to last argument is whether the values should be normalized between
	 * 0 and 1, which is set false because we don't need this.
	 *
	 * The second to last argument is the distance (in bytes) between each vertex, which
	 * is "5 * sizeof(float)" in our case.
	 *
	 * The last argument for glVertexAttribPointer is the offset from the start
	 * of the vertex in bytes.
	 */

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);
	char *data = stbi_load("images/container.jpg", &width, &height, &channels, 0);
	if (data == NULL) {
		fprintf(stderr, "fatal: failed to load container.jpg.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	vec3 camera_pos = {0.0f, 0.0f, 3.0f};
	vec3 camera_vector = {0.0f, 0.0f, 0.0f};

	mat4 model = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1},
	};

	mat4 view, projection;
	memcpy(view, model, sizeof(model));
	memcpy(projection, model, sizeof(model));

	glm_translate(model, (vec3){0.0f, 0.0f, -2.5f});
//	glm_translate(view, (vec3){0.0f, 0.0f, -3.0f});
	glm_perspective(45, WIDTH/HEIGHT, 0.1f, 100.0f, projection);

	struct timeval start, stop;
	vec3 mouse_direction;

	while (!glfwWindowShouldClose(win)) {
		gettimeofday(&start, NULL);

		get_mouse_direction(win, mouse_direction);
		process_events(win, camera_vector, mouse_direction);
		
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
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


		vec3 tmp;
		glm_vec3_add(camera_pos, mouse_direction, tmp);

		glm_translate(view, camera_vector);
		glm_lookat(camera_pos, tmp, (vec3){0.0f, 1.0f, 0.0f}, view);
		glm_rotate(model, 0.017, (vec3){0.5f, 0.5f, 0.0f});
		//glm_translate(model, (vec3){0.0f, 0.0f, -2.5f});
		//glm_translate(view, (vec3){0.0f, 0.0f, -3.0f});
		//glm_perspective(10, WIDTH/HEIGHT, 0.1f, 100.0f, projection);

		unsigned loc;
		glUseProgram(program);
	
		loc = glGetUniformLocation(program, "model");
		glUniformMatrix4fv(loc, 1, GL_FALSE, (float *) model);

		loc = glGetUniformLocation(program, "view");
		glUniformMatrix4fv(loc, 1, GL_FALSE, (float *) view);

		loc = glGetUniformLocation(program, "projection");
		glUniformMatrix4fv(loc, 1, GL_FALSE, (float *) projection);

		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		memcpy(camera_pos, camera_vector, sizeof(camera_vector));

		glfwPollEvents();
		glfwSwapBuffers(win);

		gettimeofday(&stop, NULL);
		unsigned long diff = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
		fprintf(stderr, "fps: %lf\n", (double) 1000000 / diff);
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
//	glDeleteBuffers(1, &ebo);
	destroy_shader_program(program);

	fprintf(stderr, "exiting...\n");
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
