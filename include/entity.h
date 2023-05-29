#ifndef ENTITY_H
#define ENTITY_H

#include "./common.h"

typedef struct {
	unsigned int vao;
    unsigned int texture;
	unsigned int shader_prg;
	mat4 model_matrix;
} Entity;

vbo init_vbo(float *obj_data, size_t data_size);
void destroy_vbo(vbo vbo);
Entity init_entity(vbo vbo, const char *texture, const char *v_shader, char *g_shader, const char *f_shader, vec3 position);
void render_entity(Entity entity);
void destroy_entity(Entity model);

#endif // ENTITY_H
