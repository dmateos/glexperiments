#include "model.h"

#include <cglm/cglm.h>
#include <string.h>

#include "util.h"

static void parse_obj_file(const char *path) {
    char *model_data;
    char *line;

    model_data = read_file(path);
    if (!model_data) {
        printf("could not load model %s\n", path);
        exit(1);
    }

    line = strtok(model_data, "\n");
    while (line != NULL) {
        if (strncmp(line, "vn", strlen("vn")) == 0) {
            printf("vn line: %s\n", line);
        } else if (strncmp(line, "f", strlen("f")) == 0) {
            printf("f line: %s\n", line);
        } else if (strncmp(line, "v", strlen("v")) == 0) {
            printf("v line: %s\n", line);
        } else {
            printf("unknown line: %s\n", line);
        }
        line = strtok(NULL, "\n");
    }

    free_file_data(model_data);
}

int init_model(Model *model, const char *path) {
    memset(model, 0, sizeof(Model));
    parse_obj_file(path);
    return 0;
}