import pygame
import pygame.gfxdraw
import sys
import math

SCREEN_SIZE = (1024, 1000)


class TestOrbiter(pygame.sprite.Sprite):
    def __init__(self, radius=300):
        super().__init__()
        self.image = pygame.Surface([40, 40])
        self.image.fill([255, 0, 0])
        self.rect = self.image.get_rect()
        self.theta = 1
        self.radius = radius

        self.pos_x = 1024 / 2
        self.pos_y = 768 / 2

    def update(self):
        self.rect.x = self.pos_x + self.radius * math.cos(self.theta * math.pi / 180)
        self.rect.y = self.pos_y + self.radius * math.sin(self.theta * math.pi / 180)
        self.theta += 0.5

        if self.theta >= 360:
            self.theta = 0

        print(self.theta)


class TestDirectioner(pygame.sprite.Sprite):
    def __init__(self, direction):
        super().__init__()
        self.image = pygame.Surface([40, 40])
        self.image.fill([0, 255, 0])
        self.rect = self.image.get_rect()
        self.direction = direction

        self.rect.x = 300
        self.rect.y = 300

    def update(self):
        self.rect.x += math.cos(self.direction * math.pi / 180) * 2
        self.rect.y += math.sin(self.direction * math.pi / 180) * 2


class TestBouncer(pygame.sprite.Sprite):
    def __init__(self, walls=[]):
        super().__init__()
        self.image = pygame.Surface([40, 40])
        self.image.fill([0, 0, 255])
        self.rect = self.image.get_rect()

        self.gravity = 1
        self.weight = 1
        self.bounce = 1
        self.velocity = [0, 0]

        self.walls = walls

    def update(self):
        self.velocity[1] += self.gravity

        self.rect.x += self.velocity[0]
        self.rect.y += self.velocity[1]

        for walls in self.walls:
            if walls.rect.colliderect(self.rect):
                print("bang")


class TestWall(pygame.sprite.Sprite):
    def __init__(self):
        super().__init__()
        self.image = pygame.Surface([400, 40])
        self.image.fill([0, 0, 255])
        self.rect = self.image.get_rect()

        self.rect.y += 900


class Screen:
    def __init__(self):
        self.display = pygame.display.set_mode(SCREEN_SIZE)
        self.display_surface = pygame.display.get_surface()
        self.sprite_list = pygame.sprite.RenderPlain()

        self.sprite = TestOrbiter()
        self.sprite2 = TestOrbiter(100)
        self.sprite3 = TestDirectioner(60)
        self.sprite5 = TestWall()
        self.sprite4 = TestBouncer([self.sprite5])

        self.sprite_list.add(self.sprite)
        self.sprite_list.add(self.sprite2)
        self.sprite_list.add(self.sprite3)
        self.sprite_list.add(self.sprite4)
        self.sprite_list.add(self.sprite5)

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
        # print(fps.get_fps())
