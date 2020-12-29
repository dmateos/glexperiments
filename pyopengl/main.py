# import sys
from renderer import window, primitives, shader
import glfw
import pyrr
import math


SCREEN_SIZE = (1024, 1000)
USER_COLOR = (0, 0, 1)
COMPUTER_COLOR = (0, 0, 1)
BALL_SPEED = 0.025


class Paddle:
    def __init__(self, program, computer: bool = False):
        if computer:
            self.rect = primitives.Rectangle(program, 0, 0, 0.1, 0.4, COMPUTER_COLOR)
            self.rect.x += 0.95
        else:
            self.rect = primitives.Rectangle(program, 0, 0, 0.1, 0.4, USER_COLOR)
            self.rect.x -= 0.95

    def draw(self):
        self.rect.draw()

    def computer_move(self, ball_location):
        if ball_location.y > self.rect.y:
            self.rect.y += 0.01
        else:
            self.rect.y -= 0.01


class Ball:
    def __init__(self, program):
        self.rect = primitives.Rectangle(program, 0, 0, 0.05, 0.05, (1, 0, 0))
        self.reset()

    def reset(self):
        self.direction = 0
        self.rect.x = 0
        self.rect.y = 0

    def update(self):
        self.rect.x += BALL_SPEED * math.cos(self.direction * math.pi/180)
        self.rect.y += BALL_SPEED * math.sin(self.direction * math.pi/180)

        if self.rect.y >= 1:
            self.direction -= 90
        elif self.rect.y <= -1:
            self.direction += 90

    def check_collision(self, objs):
        for n in objs:
            if n.rect.check_collision(self.rect):
                self.direction += 65

    def check_out_of_bounds(self):
        if self.rect.x <= -1:
            return "user"
        elif self.rect.x >= 1:
            return "computer"
        else:
            return "none"

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

        self.computer_paddle.computer_move(self.ball.rect)
        self.ball.update()
        self.ball.check_collision([self.user_paddle, self.computer_paddle])
        bounds = self.ball.check_out_of_bounds()

        if bounds == "user":
            self.ball.reset()
        elif bounds == "computer":
            self.ball.reset()

        for sprite in self.sprites:
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
