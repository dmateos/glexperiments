OBJS = $(wildcard *.c)
CC = gcc
INCLUDE_PATHS = -I/usr/local/include
LIBRARY_PATHS = -L/usr/local/lib
COMPILER_FLAGS = -std=c99 -Wall -O2 -D GL_SILENCE_DEPRECATION -Wextra  \
          -Wformat=2 -Wno-unused-parameter -Wshadow \
          -Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
          -Wredundant-decls -Wnested-externs -Wmissing-include-dirs

LINKER_FLAGS = -lSDL2 -lSDL2_image -l SDL2_gfx -l cglm -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
OBJ_NAME = a.out

all:
	$(CC) -o $(OBJ_NAME) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(OBJS)
test:
	./a.out models/cube.obj
