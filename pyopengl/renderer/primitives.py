import OpenGL.GL as ogl
import numpy
import pyrr


class RenderException(Exception):
    pass


class VertexState:
    """
    Maintains a draw state.
    Will track any VertexBuffers created while bound
    """

    def __init__(self) -> None:
        self.vao = ogl.glGenVertexArrays(1)

    def __enter__(self) -> None:
        self.bind()

    def __exit__(self, type, value, traceback) -> None:
        self.unbind()

    def bind(self) -> None:
        ogl.glBindVertexArray(self.vao)

    def unbind(self) -> None:
        ogl.glBindVertexArray(0)

    def draw_array(self, length: int) -> None:
        ogl.glDrawArrays(ogl.GL_TRIANGLES, 0, length)

    def draw_indexed_elements(self, length: int) -> None:
        ogl.glDrawElements(ogl.GL_TRIANGLES, length, ogl.GL_UNSIGNED_INT, None)


class VertexBuffer:
    """
    Respresents a buffer of data
    Probably should be run within a VertexState with block
    """

    def __init__(self, data: list, program, name: str) -> None:
        self.vbo = ogl.glGenBuffers(1)
        self.data = numpy.array(data, dtype="float32")
        self.name = name
        self.program = program
        self._buffer_data()

    def bind(self) -> None:
        ogl.glBindBuffer(ogl.GL_ARRAY_BUFFER, self.vbo)

    def unbind(self) -> None:
        ogl.glBindBuffer(ogl.GL_ARRAY_BUFFER, 0)

    def _buffer_data(self) -> None:
        self.bind()
        ogl.glBufferData(
            ogl.GL_ARRAY_BUFFER, len(self.data) * 4, self.data, ogl.GL_STATIC_DRAW
        )

        self.program.use()
        self.program.set_attribute(self.name)
        self.unbind()


class IndexBuffer:
    def __init__(self, data: list, program, name: str) -> None:
        self.vbo = ogl.glGenBuffers(1)
        self.data = numpy.array(data, dtype="float32")
        self.name = name
        self.program = program
        self._buffer_data()

    def bind(self) -> None:
        ogl.glBindBuffer(ogl.GL_ELEMENT_ARRAY_BUFFER, self.vbo)

    def unbind(self) -> None:
        ogl.glBindBuffer(ogl.GL_ELEMENT_ARRAY_BUFFER, 0)

    def _buffer_data(self) -> None:
        self.bind()
        ogl.glBufferData(
            ogl.GL_ELEMENT_ARRAY_BUFFER,
            len(self.data) * 4,
            self.data,
            ogl.GL_STATIC_DRAW,
        )

        self.program.use()
        self.program.set_attribute(self.name)
        self.unbind()


class Triangle:
    triangle_data = [-0.5, -0.5, 0.0, 0.5, -0.5, 0.0, 0.0, 0.5, 0.0]

    def __init__(
        self, program, translation=[0.0, 0.0, 0.0], color=[0.0, 0.0, 0.0]
    ) -> None:
        self.program = program
        self.translation = translation
        self.color = color
        self.vao = VertexState()

        with self.vao:
            VertexBuffer(self.triangle_data, program, "vp")
            VertexBuffer(self.color, program, "c")

    def draw(self) -> None:
        self.program.use()
        mat = pyrr.Matrix44.from_translation(self.translation)
        self.program.set_uniform("translation", mat)

        with self.vao:
            self.vao.draw_array(len(self.triangle_data))


class Rectangle:
    triangle_data = [
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
    ]

    def __init__(self, program, x, y, w, h, color=[0.0, 0.0, 0.0]) -> None:
        self.program = program
        self.color = color
        self.vao = VertexState()

        self.x = x
        self.y = y
        self.w = w
        self.h = h

        self.scale_matrix = pyrr.Matrix44.from_scale([self.w, self.h, 0])

        with self.vao:
            VertexBuffer(self.triangle_data, program, "vp")
            VertexBuffer(self.color, program, "c")

    def draw(self) -> None:
        self.program.use()
        mat = pyrr.Matrix44.from_translation([self.x, self.y, 0])

        self.program.set_uniform("translation", mat)
        self.program.set_uniform("scale", self.scale_matrix)

        with self.vao:
            self.vao.draw_array(len(self.triangle_data))

    def check_collision(self, rect2) -> None:
        if (
            self.x < rect2.x + rect2.w
            and self.x + self.w > rect2.x
            and self.y < rect2.y + rect2.h
            and self.y + self.h > rect2.y
        ):
            return True
        return False
