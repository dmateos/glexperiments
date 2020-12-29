# import sys
from renderer import window, primitives, shader
import glfw
import pyrr


SCREEN_SIZE = (1024, 1000)
USER_COLOR = (0, 0, 1)
COMPUTER_COLOR = (0, 0, 1)


class Paddle:
    def __init__(self, program, computer: bool = False):
        if computer:
            self.rect = primitives.Rectangle(program, 0, 0, 0.1, 0.4, COMPUTER_COLOR)
            self.rect.x += 0.50
        else:
            self.rect = primitives.Rectangle(program, 0, 0, 0.1, 0.4, USER_COLOR)
            self.rect.x -= 0.50

    def update(self):
        pass

    def draw(self):
        self.rect.draw()


class Ball:
    def __init__(self, program):
        self.rect = primitives.Rectangle(program, 0, 0, 0.05, 0.05, (1, 0, 0))
        self.rect.x += 0.25

    def update(self):
        pass

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

        perspective = pyrr.Matrix44.perspective_projection(
            75.0, 1280 / 1024, 0.1, 1000.0
        )
        self.program.set_uniform("perspective", perspective)

        self.user_paddle = Paddle(self.program)
        self.computer_paddle = Paddle(self.program, True)
        self.ball = Ball(self.program)

        self.sprites = [
            self.user_paddle,
            self.computer_paddle,
            self.ball,
        ]

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
            self.user_paddle.rect.y -= 0.1
        elif key == glfw.KEY_UP:
            self.user_paddle.rect.y += 0.1

    def mouse_handler(self, xpos, ypos):
        pass


if __name__ == "__main__":
    screen = Screen()

    while screen.ok():
        screen.update()
