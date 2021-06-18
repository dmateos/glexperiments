#include "camera.h"

#include <cglm/cglm.h>
#include <string.h>

int init_camera(Camera *camera) {
    memset(camera, 0, sizeof(Camera));
    glm_perspective(75, 1280 / 1024, 0.0, 100.0, camera->perspective);
    return 0;
}