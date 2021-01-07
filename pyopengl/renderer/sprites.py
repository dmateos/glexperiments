import pyrr
from . import primitives

TRIANGLE_DATA = (0.5, 0.5, 0.0, 0.5, -0.5, 0.0, -0.5, -0.5, 0.0, -0.5, 0.5, 0.0)
TEXTURE_COORDINATES = (1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0)
TRIANGLE_INDEXES = (0, 1, 3, 1, 2, 3)
TRIANGLE_DATA_LENGTH = len(TRIANGLE_DATA)
TRIANGLE_INDEX_LENGTH = len(TRIANGLE_INDEXES)


class Rectangle:
    def __init__(self, x, y, w, h, c):
        self.x = x
        self.y = y
        self.w = w
        self.h = h
        self.color = c


class RectangleGroup:
    """
    Used to render a group of Rectangle objects. 

    This is much more effcient for rendering large amounts of rectangles
    at the same time. 

    This uses OGL instanced arrays to push one set of VP's while batch sending every
    rectangles transformations witin the one draw call.
    """

    def __init__(self, program, sprites=[]):
        self.program = program
        self.vao = primitives.VertexState()
        self.scale_matrix = pyrr.Matrix44.from_scale([100, 100, 0])
        self.sprites = sprites
        self.colors = []

        for s in sprites:
            self.colors.append(s.rect.color[0])
            self.colors.append(s.rect.color[1])
            self.colors.append(s.rect.color[2])

        with self.vao:
            primitives.VertexBuffer(TRIANGLE_DATA, program, "vp", 3)
            primitives.IndexBuffer(TRIANGLE_INDEXES, program, "vp", 3)
            primitives.VertexBuffer(self.colors, program, "c", 3, True)
            primitives.VertexBuffer(TEXTURE_COORDINATES, program, "tx", 2)

    def draw(self):
        self.program.use()
        self.program.set_uniform("scale", self.scale_matrix)

        rects = self.update_rects()

        with self.vao:
            # TODO Update rather than new buffer each time?
            primitives.VertexBuffer(rects, self.program, "os", 3, True)
            self.vao.draw_instanced_indexed_elements(
                TRIANGLE_INDEX_LENGTH, int(len(rects) / 3)
            )

    def update_rects(self):
        rectangles = []

        for s in self.sprites:
            rectangles.append(s.rect.x)
            rectangles.append(s.rect.y)
            rectangles.append(0.0)

        return rectangles


class DrawableRectangle(Rectangle):
    """
    Renderable rectangle

    Uses a VAO and some VBO's and draws as an array.
    """

    def __init__(self, program, x, y, w, h, color=[0.0, 0.0, 0.0]) -> None:
        super().__init__(x, y, w, h, color)
        self.program = program
        self.vao = primitives.VertexState()
        self.scale_matrix = pyrr.Matrix44.from_scale([w, h, 0])

        with self.vao:
            primitives.VertexBuffer(TRIANGLE_DATA, program, "vp", 3)
            primitives.IndexBuffer(TRIANGLE_INDEXES, program, "vp", 3)
            primitives.VertexBuffer(self.color * 4, program, "c", 3)
            primitives.VertexBuffer(TEXTURE_COORDINATES, program, "tx", 2)

    def draw(self) -> None:
        self.program.use()
        mat = pyrr.Matrix44.from_translation([self.x, self.y, 0])

        self.program.set_uniform("translation", mat)
        self.program.set_uniform("scale", self.scale_matrix)

        with self.vao:
            self.vao.draw_indexed_elements(TRIANGLE_INDEX_LENGTH)

    def check_collision(self, rect2) -> None:
        if (
            self.x < rect2.x + rect2.w
            and self.x + self.w > rect2.x
            and self.y < rect2.y + rect2.h
            and self.y + self.h > rect2.y
        ):
            return True
        return False
