# import sys
from renderer import window, shader, sprites, primitives
import pyrr
import math
import random


SCREEN_SIZE = (1024, 1000)


class TestDirectioner:
    def __init__(self, program, direction):
        perms = [1, 0, 0]
        random.shuffle(perms)

        self.rect = sprites.DrawableRectangle(program, 0, 0, 100, 100, perms)
        self.direction = direction

        self.rect.x = random.randint(100, SCREEN_SIZE[0] - 100)
        self.rect.y = random.randint(100, SCREEN_SIZE[1] - 100)

    def update(self):
        self.rect.x += math.cos(self.direction * math.pi / 180) * 2
        self.rect.y += math.sin(self.direction * math.pi / 180) * 2

        if self.rect.x <= 0 or self.rect.x + self.rect.w >= SCREEN_SIZE[0]:
            self.direction = random.randint(0, 360)

        if self.rect.y <= 0 or self.rect.y + self.rect.h >= SCREEN_SIZE[1]:
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

        self.program = shader.create_program(
            shader.BASIC_VERT_SHADER, shader.BASIC_FRAG_SHADER
        )

        projection = pyrr.Matrix44.orthogonal_projection(
            0.0, SCREEN_SIZE[0], SCREEN_SIZE[1], 0.0, -1.0, 1.0
        )
        self.program.use()
        self.program.set_uniform("projection", projection)

        texture = primitives.Texture.image_from_file("assets/container.jpg")
        texture.bind()

        self.sprites = []

        for n in range(20):
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
        pass

    def mouse_handler(self, xpos, ypos):
        pass


def run():
    screen = Screen()
    while screen.ok():
        screen.update()


if __name__ == "__main__":
    # cProfile.run("run()")
    run()
