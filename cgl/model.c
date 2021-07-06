#include "model.h"

#include <cglm/cglm.h>
#include <string.h>

#include "util.h"
#include "vertex.h"

const float TEXTURE_COORDINATES[] = {1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};

static int parse_obj_file(ObjFile *obj, const char *path) {
    char *model_data, *l, *line_state;
    float x, y, z;
    unsigned int n1, n2, n3, v1, v2, v3;

    model_data = read_file(path);
    if (model_data == NULL) {
        printf("could not load model %s\n", path);
        exit(1);
    }

    l = strtok_r(model_data, "\n", &line_state);
    while (l != NULL) {
        if (strncmp(l, "vn", strlen("vn")) == 0) {
            sscanf(l, "vn %f %f %f\n", &x, &y, &z);
            printf("vn %f %f %f\n", x, y, z);
            obj->normals[obj->ncount++] = x;
            obj->normals[obj->ncount++] = y;
            obj->normals[obj->ncount++] = z;
        } else if (strncmp(l, "v", strlen("v")) == 0) {
            sscanf(l, "v %f %f %f\n", &x, &y, &z);
            printf("v %f %f %f\n", x, y, z);
            obj->verticies[obj->vcount++] = x;
            obj->verticies[obj->vcount++] = y;
            obj->verticies[obj->vcount++] = z;
        } else if (strncmp(l, "f", strlen("f")) == 0) {
            sscanf(l, "f %d//%d %d//%d %d//%d\n", &v1, &n1, &v2, &n2, &v3, &n3);
            printf("f: %d//%d %d//%d %d//%d\n", v1, n1, v2, n2, v3, n3);
            obj->verticie_index[obj->vicount++] = v1 - 1;
            obj->verticie_index[obj->vicount++] = v2 - 1;
            obj->verticie_index[obj->vicount++] = v3 - 1;
            obj->normal_index[obj->nicount++] = n1 - 1;
            obj->normal_index[obj->nicount++] = n2 - 1;
            obj->normal_index[obj->nicount++] = n3 - 1;
        }
        l = strtok_r(NULL, "\n", &line_state);
    }

    free_file_data(model_data);

    printf("loaded model %s\n", path);
    printf("vcount %d, ncount %d, vicount: %d, nicount %d\n", obj->vcount,
           obj->ncount, obj->vicount, obj->nicount);
    return 0;
}

int init_model(Model *model, const ShaderProgram *shader, const char *path,
               int instances, void *instancedata) {
    memset(model, 0, sizeof(Model));
    model->program = shader;
    model->instance_count = instances;

    parse_obj_file(&model->vdata, path);

    init_vertex_state(&model->state, VERTEX_STATE_DRAW_INDEXED);
    bind_vertex_state(&model->state);

    init_vertex_buffer(&model->vertex, VERTEX_BUFFER_TYPE_ARRAY, 0);
    bind_vertex_buffer(&model->vertex);
    write_vertex_buffer(&model->vertex, (void *)model->vdata.verticies,
                        sizeof(float) * model->vdata.vcount);

    init_vertex_buffer(&model->index, VERTEX_BUFFER_TYPE_INDEX, 0);
    bind_vertex_buffer(&model->index);
    write_vertex_buffer(&model->index, (void *)model->vdata.verticie_index,
                        sizeof(unsigned int) * model->vdata.vicount);
    set_attribute(1, 3);

    init_vertex_buffer(&model->texture, VERTEX_BUFFER_TYPE_ARRAY, 0);
    bind_vertex_buffer(&model->texture);
    write_vertex_buffer(&model->texture, (void *)TEXTURE_COORDINATES,
                        sizeof(float) * 8);

    set_attribute(3, 2);

    if (model->instance_count > 0) {
        init_vertex_buffer(&model->instance_buffer, VERTEX_BUFFER_TYPE_ARRAY,
                           1);
        bind_vertex_buffer(&model->instance_buffer);
        write_vertex_buffer(&model->instance_buffer, (void *)instancedata,
                            sizeof(float) * model->instance_count * 3);
        set_attribute(2, 3);
    }

    unbind_vertex_state(&model->state);
    return 0;
}

int draw_model(const Model *model) {
    if (model->instance_count > 0) {
        draw_instanced(&model->state, model->vdata.vicount,
                       model->instance_count);
    } else {
        glm_translate_make((vec4 *)model->translation, (float *)model->vec);
        set_uniform(get_uniform(model->program, "model"),
                    (float *)model->translation);
        draw(&model->state, model->vdata.vicount);
    }
    return 0;
}