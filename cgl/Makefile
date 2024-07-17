SRCS = $(wildcard *.c renderer/*.c base/*.c)
OBJS = $(SRCS:.c=.o)

CC = gcc
INCLUDE_PATHS = -I/opt/homebrew/include -I/usr/local/include
LIBRARY_PATHS = -L/opt/homebrew/lib -L/usr/local/lib
COMPILER_FLAGS = -std=c99 -Wall -O2 -D GL_SILENCE_DEPRECATION -Wextra \
          -Wformat=2 -Wno-unused-parameter -Wshadow \
          -Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
          -Wredundant-decls -Wnested-externs -Wmissing-include-dirs \
          -mmacosx-version-min=13.0
LINKER_FLAGS = -lSDL2 -lSDL2_image -l SDL2_gfx -l cglm -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -lAssimp
OBJ_NAME = a.out

all: $(OBJ_NAME)

$(OBJ_NAME): $(OBJS)
	$(CC) -o $@ $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(INCLUDE_PATHS) $(COMPILER_FLAGS)

test: $(OBJ_NAME)
	./$(OBJ_NAME) assets/models/cube.obj assets/textures/gridtex.png

clean:
	rm -f $(OBJS) $(OBJ_NAME)
