#include "model.h"

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cglm/cglm.h>
#include <string.h>

#include "util.h"
#include "vertex.h"

static int parse_obj_file_new(ObjFile *model, const char *path) {
  const struct aiScene *scene =
      aiImportFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (scene == NULL) {
    printf("could not import assimp file %s\n", path);
    return 1;
  }

  for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
    printf("mesh %d has %d verticies\n", i, scene->mMeshes[i]->mNumVertices);
    const struct aiMesh *mesh = scene->mMeshes[i];
    for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
      model->verticies[model->vcount++] = mesh->mVertices[j].x;
      model->verticies[model->vcount++] = mesh->mVertices[j].y;
      model->verticies[model->vcount++] = mesh->mVertices[j].z;

      model->verticies[model->vcount++] = mesh->mNormals[j].x;
      model->verticies[model->vcount++] = mesh->mNormals[j].y;
      model->verticies[model->vcount++] = mesh->mNormals[j].z;

      if (mesh->mTextureCoords[0] == NULL) {
        model->verticies[model->vcount++] = 0.0f;
        model->verticies[model->vcount++] = 0.0f;
        printf("making up texture coords\n");
      } else {
        model->verticies[model->vcount++] = mesh->mTextureCoords[0][j].x;
        model->verticies[model->vcount++] = mesh->mTextureCoords[0][j].y;
      }

      printf("vertex %d: %f %f %f\n", j, mesh->mVertices[j].x,
             mesh->mVertices[j].y, mesh->mVertices[j].z);
    }

    for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
      const struct aiFace *face = &mesh->mFaces[j];
      for (unsigned int k = 0; k < face->mNumIndices; k++) {
        model->verticie_index[model->vicount++] = face->mIndices[k];
      }
    }
  }

  aiReleaseImport(scene);
  printf("model is ok\n");
  return 0;
}

int init_model(Model *model, const ShaderProgram *shader, const char *path,
               int instances, void *instancedata) {
  memset(model, 0, sizeof(Model));
  model->program = shader;
  model->instance_count = instances;

  parse_obj_file_new(&model->vdata, path);

  init_vertex_state(&model->state, VERTEX_STATE_DRAW_INDEXED);
  bind_vertex_state(&model->state);

  // Vertex Data
  init_vertex_buffer(&model->vertex, VERTEX_BUFFER_TYPE_ARRAY, 0);
  bind_vertex_buffer(&model->vertex);
  write_vertex_buffer(&model->vertex, (void *)model->vdata.verticies,
                      sizeof(float) * model->vdata.vcount);

  // Vertex Index data
  init_vertex_buffer(&model->index, VERTEX_BUFFER_TYPE_INDEX, 0);
  bind_vertex_buffer(&model->index);
  write_vertex_buffer(&model->index, (void *)model->vdata.verticie_index,
                      sizeof(unsigned int) * model->vdata.vicount);
  set_attribute(get_attribute(shader, "vp"), 3, 8, 0);
  set_attribute(get_attribute(shader, "nm"), 3, 8, 3);
  set_attribute(get_attribute(shader, "tx"), 2, 8, 6);

  // Instance offset data (if applicable)
  if (model->instance_count > 0) {
    init_vertex_buffer(&model->instance_buffer, VERTEX_BUFFER_TYPE_ARRAY, 1);
    bind_vertex_buffer(&model->instance_buffer);
    write_vertex_buffer(&model->instance_buffer, (void *)instancedata,
                        sizeof(float) * model->instance_count * 3);
    set_attribute(get_attribute(shader, "os"), 3, 0, 0);
  }

  unbind_vertex_state(&model->state);
  return 0;
}

int draw_model(const Model *model) {
  if (model->instance_count > 0) {
    draw_instanced(&model->state, model->vdata.vicount, model->instance_count);
  } else {
    glm_translate_make((vec4 *)model->translation, (float *)model->vec);
    set_uniform(get_uniform(model->program, "model"),
                (float *)model->translation);
    draw(&model->state, model->vdata.vicount);
  }
  return 0;
}