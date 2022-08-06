#ifndef _UTILS_H
#define _UTILS_H

#define OBJ_VERTEX_LIMIT 102400

typedef struct _Obj {
  float verticies[OBJ_VERTEX_LIMIT], normals[OBJ_VERTEX_LIMIT];
  unsigned int verticie_index[OBJ_VERTEX_LIMIT];
  unsigned int normal_index[OBJ_VERTEX_LIMIT];
  unsigned int ncount, vcount, vicount, nicount;
} ObjFile;

int parse_obj_file(ObjFile *obj, const char *path);

#endif