#include "model.h"

#include <cglm/cglm.h>
#include <string.h>

#include "util.h"

static void parse_obj_file(const char *path) {
    char *model_data;

    model_data = read_file(path);
    if (!model_data) {
        printf("could not load model %s\n", path);
        exit(1);
    }

    printf("%s\n", model_data);
    free_file_data(model_data);
}

int init_model(Model *model, const char *path) {
    memset(model, 0, sizeof(Model));
    parse_obj_file(path);
    return 0;
}