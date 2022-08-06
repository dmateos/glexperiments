#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *read_file(const char *filename) {
  char *buffer = 0;
  long length;
  FILE *f = fopen(filename, "rb");

  if (f) {
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    buffer = malloc(length + 1);
    memset(buffer, 0, length + 1);
    if (buffer) {
      fread(buffer, 1, length, f);
    }
    fclose(f);
  }
  return buffer;
}

static void free_file_data(char *filedata) { free(filedata); }

int parse_obj_file(ObjFile *obj, const char *path) {
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
      // Vertex Normals
      sscanf(l, "vn %f %f %f\n", &x, &y, &z);
      // printf("vn %f %f %f\n", x, y, z);
      obj->normals[obj->ncount++] = x;
      obj->normals[obj->ncount++] = y;
      obj->normals[obj->ncount++] = z;
    } else if (strncmp(l, "v", strlen("v")) == 0) {
      // Vertex
      sscanf(l, "v %f %f %f\n", &x, &y, &z);
      // printf("v %f %f %f\n", x, y, z);
      obj->verticies[obj->vcount++] = x;
      obj->verticies[obj->vcount++] = y;
      obj->verticies[obj->vcount++] = z;
    } else if (strncmp(l, "f", strlen("f")) == 0) {
      // Index's for above split by //
      sscanf(l, "f %d//%d %d//%d %d//%d\n", &v1, &n1, &v2, &n2, &v3, &n3);
      // printf("f: %d//%d %d//%d %d//%d\n", v1, n1, v2, n2, v3, n3);
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