# import sys
from renderer import window, shader, sprites, primitives
import glfw
import pyrr
import math


SCREEN_SIZE = (1024, 1000)
USER_COLOR = (0, 0, 1)
COMPUTER_COLOR = (0, 0, 1)
BALL_COLOR = (1, 0, 0)
BALL_SPEED = 10


class Paddle:
    def __init__(self, program, computer: bool = False):
        if computer:
            self.rect = sprites.DrawableRectangle(
                program, 0, 0, 40, 100, COMPUTER_COLOR
            )
            self.rect.x += SCREEN_SIZE[0] - 80
        else:
            self.rect = sprites.DrawableRectangle(program, 0, 0, 40, 100, USER_COLOR)
            self.rect.x += 40

    def draw(self):
        self.rect.draw()

    def computer_move(self, ball_location):
        if ball_location.y > self.rect.y and self.rect.y + 100 <= SCREEN_SIZE[1]:
            self.rect.y += 10
        else:
            self.rect.y -= 10


class Ball:
    def __init__(self, program):
        self.rect = sprites.DrawableRectangle(program, 0, 0, 40, 40, BALL_COLOR)
        self.reset()

    def reset(self):
        self.direction = 0
        self.rect.x = 500
        self.rect.y = 500

    def update(self):
        self.rect.x += BALL_SPEED * math.cos(self.direction * math.pi / 180)
        self.rect.y += BALL_SPEED * math.sin(self.direction * math.pi / 180)

        if self.rect.y + 40 >= SCREEN_SIZE[1]:
            self.direction -= 90
        elif self.rect.y <= 0:
            self.direction += 90

    def check_collision(self, objs):
        for n in objs:
            if n.rect.check_collision(self.rect):
                self.direction += 65

    def check_out_of_bounds(self):
        if self.rect.x <= 0:
            return "user"
        elif self.rect.x >= SCREEN_SIZE[0]:
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

        self.program = shader.create_program(
            shader.BASIC_VERT_SHADER, shader.BASIC_FRAG_SHADER
        )

        projection = pyrr.Matrix44.orthogonal_projection(
            0.0, SCREEN_SIZE[0], SCREEN_SIZE[1], 0.0, -1.0, 1.0
        )
        self.program.set_uniform("projection", projection)

        texture = primitives.Texture.image_from_file("assets/container.jpg")
        texture.bind()

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
        self.user_paddle.computer_move(self.ball.rect)
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
            self.user_paddle.rect.y += 50
        elif key == glfw.KEY_UP:
            self.user_paddle.rect.y -= 50

    def mouse_handler(self, xpos, ypos):
        pass


if __name__ == "__main__":
    screen = Screen()

    while screen.ok():
        screen.update()
