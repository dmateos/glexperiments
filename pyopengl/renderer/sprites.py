import OpenGL.GL as ogl
import pyrr
from . import primitives

TRIANGLE_DATA = (
    -0.5,
    -0.5,
    0,
    0.5,
    -0.5,
    0,
    -0.5,
    0.5,
    0,
    0.5,
    0.5,
    0,
    -0.5,
    0.5,
    0,
    0.5,
    -0.5,
    0,
)


TRIANGLE_DATA_LENGTH = len(TRIANGLE_DATA)


class RectangleGroup:
    def __init__(self, program, sprites=[]):
        self.vao = primitives.VertexState()
        self.program = program
        self.scale_matrix = pyrr.Matrix44.from_scale([4, 4, 0])

        self.sprites = sprites

        with self.vao:
            primitives.VertexBuffer(TRIANGLE_DATA, program, "vp")
            primitives.VertexBuffer((0, 0, 1), program, "c")

    def append(self, sprite):
        self.sprites.append(sprite)

    def draw(self):
        self.program.use()
        self.program.set_uniform("scale", self.scale_matrix)

        rects = self.update_rects()

        with self.vao:
            # TODO Update rather than new buffer each time?
            primitives.VertexBuffer(rects, self.program, "os", True)
            self.vao.draw_instanced(TRIANGLE_DATA_LENGTH, int(len(rects) / 3))

    def update_rects(self):
        rectangles = []

        for s in self.sprites:
            rectangles.append(s.rect.x)
            rectangles.append(s.rect.y)
            rectangles.append(0.0)

        return rectangles


class Rectangle:
    def __init__(self, program, x, y, w, h, color=[0.0, 0.0, 0.0]) -> None:
        self.program = program
        self.color = color
        self.vao = primitives.VertexState()

        self.x = x
        self.y = y
        self.w = w
        self.h = h

        self.scale_matrix = pyrr.Matrix44.from_scale([self.w, self.h, 0])

        with self.vao:
            primitives.VertexBuffer(TRIANGLE_DATA, program, "vp")
            primitives.VertexBuffer(self.color, program, "c")

    def draw(self) -> None:
        self.program.use()
        mat = pyrr.Matrix44.from_translation([self.x, self.y, 0])

        self.program.set_uniform("translation", mat)
        self.program.set_uniform("scale", self.scale_matrix)

        with self.vao:
            self.vao.draw_array(TRIANGLE_DATA_LENGTH)

    def check_collision(self, rect2) -> None:
        if (
            self.x < rect2.x + rect2.w
            and self.x + self.w > rect2.x
            and self.y < rect2.y + rect2.h
            and self.y + self.h > rect2.y
        ):
            return True
        return False
