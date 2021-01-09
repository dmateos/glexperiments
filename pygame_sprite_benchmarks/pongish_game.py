import pygame
import pygame.gfxdraw
import sys
import math
import numpy
from typing import List


SCREEN_SIZE = (1024, 1000)
FIELD_SIZE = (1024, 768)
USER_COLOR = (0, 0, 255)
COMPUTER_COLOR = (0, 0, 255)
BACKGROUND_COLOR = (0, 0, 0)
BALL_SPEED = 10


class Paddle(pygame.sprite.Sprite):
    def __init__(self, computer: bool = False):
        super().__init__()
        self.image = pygame.Surface([40, 100])
        self.rect = self.image.get_rect()
        self.direction = numpy.array((1, 0))

        if computer:
            self.image.fill(COMPUTER_COLOR)
            self.rect.x += FIELD_SIZE[0] - 42
            self.direction = numpy.array((-1, 0))
        else:
            self.image.fill(USER_COLOR)
            self.rect.x = 2

    def computer_move(self, ball_location) -> None:
        if ball_location.y > self.rect.y and self.rect.y + 100 <= FIELD_SIZE[1]:
            self.rect.y += 10
        else:
            self.rect.y -= 10

    def move(self, offset: int) -> None:
        if self.rect.y + offset <= FIELD_SIZE[1] and self.rect.y + offset >= 0:
            self.rect.y += offset


class Ball(pygame.sprite.Sprite):
    def __init__(self):
        super().__init__()
        self.image = pygame.Surface([40, 40])
        self.image.fill([255, 0, 0])
        self.rect = self.image.get_rect()
        self.reset()

    def reset(self) -> None:
        self.rect.x = 100
        self.rect.y = 100
        self.direction = 0

    def update(self) -> None:
        self.rect.x += BALL_SPEED * math.cos(self.direction * math.pi / 180)
        self.rect.y += BALL_SPEED * math.sin(self.direction * math.pi / 180)

        if self.rect.y + 40 >= FIELD_SIZE[1]:
            self.direction += 90
        elif self.rect.y <= 0:
            self.direction -= 90

    def check_collision(self, objs: List) -> None:
        for n in objs:
            if n.rect.colliderect(self.rect):
                self.direction += 65

    def check_out_of_bounds(self) -> str:
        if self.rect.x <= 0:
            return "user"
        elif self.rect.x >= FIELD_SIZE[0]:
            return "computer"
        else:
            return "none"


class ScoreBoard:
    def __init__(self):
        self.font = pygame.font.Font(None, 100)
        self.user_score = 0
        self.computer_score = 0
        self.text = "{} vs {}"

    def user_win(self) -> None:
        self.user_score += 1

    def computer_win(self) -> None:
        self.computer_score += 1

    def render(self, text: str) -> pygame.Surface:
        return self.font.render(
            self.text.format(self.user_score, self.computer_score),
            True,
            (0, 255, 0),
            None,
        )


class Screen:
    def __init__(self):
        self.display = pygame.display.set_mode(SCREEN_SIZE)
        self.display_surface = pygame.display.get_surface()
        self.display_rect = pygame.Rect(0, 0, FIELD_SIZE[0], FIELD_SIZE[1])
        self.sprite_list = pygame.sprite.RenderPlain()

        self.ball = Ball()
        self.user_paddle = Paddle()
        self.computer_paddle = Paddle(True)
        self.score_board = ScoreBoard()

        self.sprite_list.add(self.user_paddle)
        self.sprite_list.add(self.computer_paddle)
        self.sprite_list.add(self.ball)

    def update(self) -> None:
        self.display.fill(BACKGROUND_COLOR)
        pygame.gfxdraw.rectangle(
            self.display,
            pygame.Rect(1, 1, FIELD_SIZE[0] - 1, FIELD_SIZE[1]),
            (0, 255, 0),
        )

        self.computer_paddle.computer_move(self.ball.rect)
        self.ball.update()
        self.ball.check_collision((self.user_paddle, self.computer_paddle))
        bounds = self.ball.check_out_of_bounds()

        if bounds == "user":
            self.score_board.computer_win()
            self.ball.reset()
        elif bounds == "computer":
            self.score_board.user_win()
            self.ball.reset()

        self.sprite_list.draw(self.display)
        self.display_surface.blit(self.score_board.render("test"), (0, FIELD_SIZE[1]))
        pygame.display.flip()

    def handle_user_event(self, key: int):
        if key == pygame.K_DOWN:
            self.user_paddle.move(100)
        elif key == pygame.K_UP:
            self.user_paddle.move(-100)


if __name__ == "__main__":
    pygame.init()
    screen = Screen()
    fps = pygame.time.Clock()

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                sys.exit(0)
            elif event.type == pygame.KEYDOWN:
                screen.handle_user_event(event.key)

        screen.update()
        fps.tick(60)
        # print(fps.get_fps())
