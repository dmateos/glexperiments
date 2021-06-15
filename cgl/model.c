#include "model.h"

#include <cglm/cglm.h>
#include <string.h>

#include "util.h"

static void parse_obj_file(const char *path) {
    char *model_data, *line, *tok;
    char *line_state, *token_state = NULL;

    model_data = read_file(path);
    if (!model_data) {
        printf("could not load model %s\n", path);
        exit(1);
    }

    line = strtok_r(model_data, "\n", &line_state);
    while (line != NULL) {
        if (strncmp(line, "vn", strlen("vn")) == 0) {
            printf("vn line: %s\n", line);
            tok = strtok_r(line, " ", &token_state);
            tok = strtok_r(NULL, " ", &token_state);
            while (tok != NULL) {
                printf("vn token: %s\n", tok);
                if (tok[strlen(tok)] == '\n') {
                    tok = NULL;
                } else {
                    tok = strtok_r(NULL, " ", &token_state);
                }
            }
        } else if (strncmp(line, "v", strlen("v")) == 0) {
            printf("v line: %s\n", line);
            tok = strtok_r(line, " ", &token_state);
            tok = strtok_r(NULL, " ", &token_state);
            while (tok != NULL) {
                printf("v token: %s\n", tok);
                if (tok[strlen(tok)] == '\n') {
                    tok = NULL;
                } else {
                    tok = strtok_r(NULL, " ", &token_state);
                }
            }
        } else if (strncmp(line, "f", strlen("f")) == 0) {
            printf("f line: %s\n", line);
        } else {
            printf("unknown line: %s\n", line);
        }
        line = strtok_r(NULL, "\n", &line_state);
        token_state = NULL;
    }

    free_file_data(model_data);
}

int init_model(Model *model, const char *path) {
    memset(model, 0, sizeof(Model));
    parse_obj_file(path);
    return 0;
}