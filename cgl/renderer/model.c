#include "model.h"

#include <OpenGL/gl3.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cglm/cglm.h>
#include <string.h>

#include "util.h"
#include "vertex.h"

static int parse_obj_file(ObjFile *model, const char *path) {
  const struct aiScene *scene =
      aiImportFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (scene == NULL) {
    printf("could not import assimp file %s\n", path);
    return 1;
  }

  for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
    printf("mesh %d has %d verticies\n", i, scene->mMeshes[i]->mNumVertices);
    model->verticies =
        malloc(sizeof(float) * scene->mMeshes[i]->mNumVertices * 8);

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

      /*
        printf("vertex %d: %f %f %f\n", j, mesh->mVertices[j].x,
        mesh->mVertices[j].y, mesh->mVertices[j].z);
      */
    }

    // todo maybe fix
    model->verticie_index = malloc(sizeof(unsigned int) * mesh->mNumFaces * 3);
    for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
      const struct aiFace *face = &mesh->mFaces[j];
      for (unsigned int k = 0; k < face->mNumIndices; k++) {
        model->verticie_index[model->vicount++] = face->mIndices[k];
      }
    }
  }

  aiReleaseImport(scene);
  printf("mesh data %s, vcount: %d, vicount: %d\n", path, model->vcount,
         model->vicount);
  return 0;
}

int model_init(Model *model, const ShaderProgram *shader, const char *path,
               const char *tpath, int instances, void *instancedata) {
  memset(model, 0, sizeof(Model));
  model->program = shader;
  model->instance_count = instances;

  parse_obj_file(&model->vdata, path);

  vertex_init_state(&model->state, VERTEX_STATE_DRAW_INDEXED);
  vertex_bind_state(&model->state);

  // Vertex Data
  vertex_init_buffer(&model->vertex, VERTEX_BUFFER_TYPE_ARRAY, 0);
  vertex_bind_buffer(&model->vertex);
  vertex_write_buffer(&model->vertex, (void *)model->vdata.verticies,
                      sizeof(float) * model->vdata.vcount);

  // Vertex Index data
  vertex_init_buffer(&model->index, VERTEX_BUFFER_TYPE_INDEX, 0);
  vertex_bind_buffer(&model->index);
  vertex_write_buffer(&model->index, (void *)model->vdata.verticie_index,
                      sizeof(unsigned int) * model->vdata.vicount);
  shader_set_attribute(shader_get_attribute(shader, "vp"), 3, 8, 0);
  shader_set_attribute(shader_get_attribute(shader, "nm"), 3, 8, 3);
  shader_set_attribute(shader_get_attribute(shader, "tx"), 2, 8, 6);

  // Instance offset data (if applicable)
  if (model->instance_count > 0) {
    vertex_init_buffer(&model->instance_buffer, VERTEX_BUFFER_TYPE_ARRAY, 1);
    vertex_bind_buffer(&model->instance_buffer);
    vertex_write_buffer(&model->instance_buffer, (void *)instancedata,
                        sizeof(float) * model->instance_count * 3);
    glVertexAttribDivisor(shader_get_attribute(shader, "os"), 1);
    shader_set_attribute(shader_get_attribute(shader, "os"), 3, 0, 0);
  }

  texture_init(&model->texture, tpath);
  vertex_unbind_state(&model->state);
  printf("initialized model %s\n", path);
  return 0;
}

int model_draw(const Model *model) {
  texture_bind(&model->texture);

  if (model->instance_count > 0) {
    vertex_draw_instanced(&model->state, model->vdata.vicount,
                          model->instance_count);
  } else {
    glm_translate_make((vec4 *)model->translation, (float *)model->vec);
    shader_set_uniform(shader_get_uniform(model->program, "model"),
                       (float *)model->translation);
    vertex_draw(&model->state, model->vdata.vicount);
  }
  return 0;
}

/*
int model_proximity(Model *model, float *vec, float radius) {
  float dist = glm_vec3_distance((vec3 *)model->vec, (vec3 *)vec);
  if (dist < radius) {
    return 1;
  } else {
    return 0;
  }
}
*/

void model_free(Model *model) {
  free(model->vdata.verticies);
  free(model->vdata.verticie_index);
  printf("freed model\n");
}