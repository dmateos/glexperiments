#include "model.h"

#include <cglm/cglm.h>
#include <string.h>

#include "util.h"

static int parse_obj_file(ObjFile *obj, const char *path) {
    char *model_data, *line, *line_state;
    float x, y, z;
    unsigned int n1, n2, n3, v1, v2, v3;

    memset(obj, 0, sizeof(ObjFile));
    model_data = read_file(path);
    if (!model_data) {
        printf("could not load model %s\n", path);
        exit(1);
    }

    line = strtok_r(model_data, "\n", &line_state);
    while (line != NULL) {
        if (strncmp(line, "vn", strlen("vn")) == 0) {
            sscanf(line, "vn %f %f %f\n", &x, &y, &z);
            printf("vn %f %f %f\n", x, y, z);
            obj->normals[obj->ncount++] = x;
            obj->normals[obj->ncount++] = y;
            obj->normals[obj->ncount++] = z;
        } else if (strncmp(line, "v", strlen("v")) == 0) {
            sscanf(line, "v %f %f %f\n", &x, &y, &z);
            printf("v %f %f %f\n", x, y, z);
            obj->verticies[obj->vcount++] = x;
            obj->verticies[obj->vcount++] = y;
            obj->verticies[obj->vcount++] = z;
        } else if (strncmp(line, "f", strlen("f")) == 0) {
            sscanf(line, "f %d//%d %d//%d %d//%d\n", &v1, &n1, &v2, &n2, &v3,
                   &n3);
            printf("f: %d//%d %d//%d %d//%d\n", v1, n1, v2, n2, v3, n3);
            obj->verticie_index[obj->vicount++] = v1;
            obj->verticie_index[obj->vicount++] = v2;
            obj->verticie_index[obj->vicount++] = v3;
            obj->normal_index[obj->nicount++] = n1;
            obj->normal_index[obj->nicount++] = n2;
            obj->normal_index[obj->nicount++] = n3;
        }
        line = strtok_r(NULL, "\n", &line_state);
    }

    free_file_data(model_data);
    return 0;
}

int init_model(Model *model, const char *path) {
    ObjFile obj;
    memset(model, 0, sizeof(Model));
    parse_obj_file(&obj, path);
    return 0;
}