OBJS = $(wildcard *.c)
CC = gcc
INCLUDE_PATHS = -I/usr/local/include
LIBRARY_PATHS = -L/usr/local/lib
COMPILER_FLAGS = -std=c99 -Wall -O2 -D GL_SILENCE_DEPRECATION
LINKER_FLAGS = -lSDL2 -lSDL2_image -l SDL2_gfx -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
OBJ_NAME = a.out

all:
	$(CC) -o $(OBJ_NAME) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(OBJS)
