# import sys
from renderer import window, shader, sprites, primitives
import pyrr
import math
import random

SCREEN_SIZE = (1000, 1000)
FRAME_SIZE = (500, 500)


class TestDirectioner:
    def __init__(self, program, direction):
        perms = [1, 0, 0]
        random.shuffle(perms)

        self.rect = sprites.DrawableRectangle(program, 0, 0, 250, 250, perms)
        self.direction = direction

        self.rect.x = random.randint(0, FRAME_SIZE[0])
        self.rect.y = random.randint(0, FRAME_SIZE[1])

    def update(self):
        self.rect.x += math.cos(self.direction * math.pi / 180) * 2
        self.rect.y += math.sin(self.direction * math.pi / 180) * 2

        if self.rect.x <= 0 or self.rect.x >= FRAME_SIZE[0]:
            self.direction = random.randint(0, 360)

        if self.rect.y <= 0 or self.rect.y >= FRAME_SIZE[1]:
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

        screen_projection = pyrr.Matrix44.orthogonal_projection(
            0.0, SCREEN_SIZE[0], SCREEN_SIZE[1], 0.0, -1.0, 1.0
        )

        buffer_projection = pyrr.Matrix44.orthogonal_projection(
            0.0, FRAME_SIZE[0], FRAME_SIZE[1], 0.0, -1.0, 1.0
        )

        self.instanced_program = shader.create_program(
            "renderer/shaders/instanced_vert.gsl", shader.BASIC_FRAG_SHADER
        )
        self.instanced_program.set_uniform("projection", buffer_projection)

        self.normal_program = shader.create_program(
            shader.BASIC_VERT_SHADER, shader.BASIC_FRAG_SHADER
        )
        self.normal_program.set_uniform("projection", screen_projection)

        self.framebuffer = primitives.FrameBuffer(FRAME_SIZE[0], FRAME_SIZE[1])

        self.sprite_texture = primitives.Texture.image_from_file("assets/container.jpg")
        self.sprite_texture.bind()

        self.sprites = []

        for n in range(10):
            sprite = TestDirectioner(self.instanced_program, random.randint(0, 360))
            self.sprites.append(sprite)

        self.sprite_group = sprites.RectangleGroup(
            self.instanced_program, self.sprites, 40, 40
        )
        self.render_quad = sprites.DrawableRectangle(
            self.normal_program,
            FRAME_SIZE[0] / 2,
            FRAME_SIZE[1] / 2,
            FRAME_SIZE[0],
            FRAME_SIZE[1],
            [1, 1, 1],
        )

        self.render_quad2 = sprites.DrawableRectangle(
            self.normal_program,
            FRAME_SIZE[0] / 2 + FRAME_SIZE[0],
            FRAME_SIZE[1] / 2 + FRAME_SIZE[1],
            FRAME_SIZE[0],
            FRAME_SIZE[1],
            [1, 1, 1],
        )

    def update(self):
        self.sprite_texture.bind()
        for sprite in self.sprites:
            sprite.update()

        with self.framebuffer:
            self.window.set_clear_color(0, 1, 0)
            self.window.clear()
            self.sprite_group.draw()

        self.window.set_clear_color(1, 0, 0)
        self.window.clear()
        self.framebuffer.bind_texture()

        self.render_quad.draw()
        self.render_quad2.draw()

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
    run()
