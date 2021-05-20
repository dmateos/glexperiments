import pygame
import pygame.gfxdraw
import sys
import math
import random

SCREEN_SIZE = (1024, 1000)


class TestDirectioner(pygame.sprite.Sprite):
    def __init__(self, direction):
        super().__init__()
        self.image = pygame.Surface([4, 4])
        self.image.fill([0, 255, 0])
        self.rect = self.image.get_rect()
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


class Screen:
    def __init__(self):
        self.display = pygame.display.set_mode(SCREEN_SIZE)
        self.display_surface = pygame.display.get_surface()
        self.sprite_list = pygame.sprite.RenderPlain()

        for n in range(20000):
            sprite = TestDirectioner(random.randint(0, 360))
            self.sprite_list.add(sprite)

    def update(self):
        self.display.fill((0, 0, 0))
        self.sprite_list.update()
        self.sprite_list.draw(self.display)
        pygame.display.flip()

    def handle_user_event(self, key):
        if key == pygame.K_DOWN:
            self.sprite.direction += 1


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
        print(fps.get_fps())
