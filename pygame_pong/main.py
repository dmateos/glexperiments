import pygame
import sys


SCREEN_SIZE = (1024, 768)
USER_COLOR = (0, 0, 255)
COMPUTER_COLOR = (0, 255, 0)
BALL_SPEED = 4


class Paddle(pygame.sprite.Sprite):
    def __init__(self, computer=False):
        super().__init__()
        self.computer = computer
        self.image = pygame.Surface([40, 100])
        self.rect = self.image.get_rect()

        if computer:
            self.image = pygame.Surface([40, 800])  # TEMP
            self.rect = self.image.get_rect()
            self.image.fill(USER_COLOR)
            self.rect.x += SCREEN_SIZE[0] - 40
        else:
            self.image.fill(COMPUTER_COLOR)
            self.rect.x = 0

    def update(self):
        if not self.computer:
            return

    def move(self, offset):
        self.rect.y += offset


class Ball(pygame.sprite.Sprite):
    def __init__(self):
        super().__init__()
        self.image = pygame.Surface([40, 40])
        self.image.fill([255, 0, 0])
        self.rect = self.image.get_rect()
        self.rect.x += 100
        self.velocity = [BALL_SPEED, BALL_SPEED]

    def update(self):
        self.rect.x += self.velocity[0]
        self.rect.y += self.velocity[1]

        if self.rect.y + 40 >= SCREEN_SIZE[1]:
            self.velocity[1] *= -1
        elif self.rect.y <= 0:
            self.velocity[1] *= -1

    def check_collision(self, objs):
        for n in objs:
            if n.rect.colliderect(self.rect):
                self.velocity[0] *= -1

    def check_out_of_bounds(self):
        if self.rect.x <= 0:
            return "user"
        elif self.rect.x >= SCREEN_SIZE[0]:
            return "computer"
        else:
            return "none"

    def reset(self):
        self.rect.x = 100
        self.rect.y = 100
        self.velocity = [BALL_SPEED, BALL_SPEED]


class Screen:
    def __init__(self):
        self.display = pygame.display.set_mode(SCREEN_SIZE)
        self.display_rect = pygame.Rect(0, 0, 1024, 768)
        self.sprite_list = pygame.sprite.RenderPlain()

        self.ball = Ball()
        self.user_paddle = Paddle()
        self.computer_paddle = Paddle(True)

        self.sprite_list.add(self.user_paddle)
        self.sprite_list.add(self.computer_paddle)
        self.sprite_list.add(self.ball)

    def update(self):
        self.display.fill((0, 0, 0))
        self.sprite_list.update()
        self.ball.check_collision((self.user_paddle, self.computer_paddle))

        bounds = self.ball.check_out_of_bounds()
        if bounds == "user":
            print("computer point")
            self.ball.reset()
        elif bounds == "computer":
            print("user point")
            self.ball.reset()

        self.sprite_list.draw(self.display)
        pygame.display.flip()

    def handle_user_event(self, key):
        if key == pygame.K_DOWN:
            self.user_paddle.move(100)
        elif key == pygame.K_UP:
            self.user_paddle.move(-100)


if __name__ == "__main__":
    pygame.init()
    world = Screen()

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                sys.exit(0)
            elif event.type == pygame.KEYDOWN:
                world.handle_user_event(event.key)

        world.update()
