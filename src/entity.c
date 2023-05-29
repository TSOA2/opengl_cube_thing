#include "../include/entity.h"
#include "../include/shaders.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stbi.h"

static void load_texture(Entity *entity, const char *path)
{
	int width, height, channels;
	unsigned char *data;
	
	glGenTextures(1, &entity->texture);
	glBindTexture(GL_TEXTURE_2D, entity->texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(1);
	data = (unsigned char *) stbi_load(path, &width, &height, &channels, 0);
	if (data == NULL) {
		fprintf(stderr, "fatal: failed to load %s\n", path);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);
}

vbo init_vbo(float *obj_data, size_t data_size)
{
	vbo vbo;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data_size, obj_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	return vbo;
}

void destroy_vbo(vbo vbo)
{
	glDeleteBuffers(1, &vbo);
}

Entity init_entity(vbo vbo, const char *texture, const char *v_shader, char *g_shader, const char *f_shader, vec3 position)
{
	int v_object, g_object, f_object;

	Entity return_model;

	glGenVertexArrays(1, &return_model.vao);

	glBindVertexArray(return_model.vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

    load_texture(&return_model, texture);

	if (g_shader) {
		int shader_array[3];
		v_object = compile_shader(v_shader, GL_VERTEX_SHADER);
		g_object = compile_shader(g_shader, GL_GEOMETRY_SHADER);
		f_object = compile_shader(f_shader, GL_FRAGMENT_SHADER);
		shader_array[0] = v_object;
		shader_array[1] = g_object;
		shader_array[2] = f_object;
		return_model.shader_prg = crt_shader_program(shader_array, 3);
	} else {
		int shader_array[2];
		v_object = compile_shader(v_shader, GL_VERTEX_SHADER);
		f_object = compile_shader(f_shader, GL_FRAGMENT_SHADER);
		shader_array[0] = v_object;
		shader_array[1] = f_object;
		return_model.shader_prg = crt_shader_program(shader_array, 2);
	}

	glm_mat4_identity(return_model.model_matrix);
	glm_translate_make(return_model.model_matrix, position);
	return return_model;
}

void render_entity(Entity entity)
{
    glUseProgram(entity.shader_prg);

	add_uniform_mat4(entity.shader_prg, "model", entity.model_matrix);
    glBindTexture(GL_TEXTURE_2D, entity.texture);
    glBindVertexArray(entity.vao);

}

void destroy_entity(Entity entity)
{
	glDeleteVertexArrays(1, &entity.vao);
	destroy_shader_program(entity.shader_prg);
}
