# import sys
from renderer import window, primitives, shader
import glfw
import pyrr
import math
import random

SCREEN_SIZE = (1024, 1000)


class TestDirectioner:
    def __init__(self, program, direction):
        super().__init__()
        self.rect = primitives.Rectangle(program, 0, 0, 4, 4, [0, 0, 1])
        self.direction = direction

        self.rect.x = random.randint(0, SCREEN_SIZE[0])
        self.rect.y = random.randint(0, SCREEN_SIZE[1])

    def update(self):
        self.rect.x += math.cos(self.direction * math.pi / 180) * 2
        self.rect.y += math.sin(self.direction * math.pi / 180) * 2

        if self.rect.x <= 0 or self.rect.x >= SCREEN_SIZE[0]:
            self.direction = random.randint(0, 360)

        if self.rect.y <= 0 or self.rect.y >= SCREEN_SIZE[1]:
            self.direction = random.randint(0, 360)

    def draw(self):
        self.rect.draw()


class Screen:
    def __init__(self):
        self.window = window.Window(SCREEN_SIZE[0], SCREEN_SIZE[1])
        self.window.run()

        self.window.add_key_callback(self.key_handler)
        self.window.reset_mouse_position()
        self.window.add_mouse_callback(self.mouse_handler)

        with open("renderer/shaders/basic_frag.gsl") as f:
            frag_data = f.read()
        with open("renderer/shaders/basic_vert.gsl") as f:
            vert_data = f.read()

        basic_frag = shader.Shader("fragment", frag_data)
        basic_vert = shader.Shader("vertex", vert_data)

        self.program = shader.Program()

        self.program.add_shader(basic_frag)
        self.program.add_shader(basic_vert)
        self.program.validate()
        self.program.compile()
        self.program.use()

        projection = pyrr.Matrix44.orthogonal_projection(
            0.0, SCREEN_SIZE[0], SCREEN_SIZE[1], 0.0, -1.0, 1.0
        )
        self.program.set_uniform("projection", projection)

        self.sprites = []

        for n in range(2000):
            sprite = TestDirectioner(self.program, random.randint(0, 360))
            self.sprites.append(sprite)

    def update(self):
        self.window.clear()

        for sprite in self.sprites:
            sprite.update()
            sprite.draw()

        self.window.swap()

    def ok(self):
        return self.window.ok()

    def key_handler(self, key, code, action, mods):
        if key == glfw.KEY_DOWN:
            self.user_paddle.rect.y += 50
        elif key == glfw.KEY_UP:
            self.user_paddle.rect.y -= 50

    def mouse_handler(self, xpos, ypos):
        pass


if __name__ == "__main__":
    screen = Screen()

    while screen.ok():
        screen.update()
