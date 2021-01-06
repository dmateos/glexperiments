import OpenGL.GL as ogl
import numpy
import ctypes
from PIL import Image


class RenderException(Exception):
    pass


class VertexState:
    """
    Maintains a draw state.
    VertexArray in opengl. 
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

    def draw_instanced(self, length: int, instances: int) -> None:
        ogl.glDrawArraysInstanced(ogl.GL_TRIANGLES, 0, length, instances)

    def draw_instanced_indexed_elements(self, length: int, instances: int) -> None:
        ogl.glDrawElementsInstanced(
            ogl.GL_TRIANGLES, length, ogl.GL_UNSIGNED_INT, None, instances
        )


class VertexBuffer:
    """
    Respresents a buffer of data
    Probably should be run within a VertexState with block
    Using this class may fuck with openGL state not leaving it how it found it. 
    """

    def __init__(
        self, data: list, program, name: str, stepping=3, instanced=False
    ) -> None:
        self.vbo = ogl.glGenBuffers(1)
        self.data = numpy.array(data, dtype="float32")
        self.name = name
        self.program = program
        self.instanced = instanced
        self.stepping = stepping
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
        self.program.set_attribute(self.name, self.stepping)

        # If instanced this attribute is split between verticie instances.
        if self.instanced:
            ogl.glVertexAttribDivisor(self.program.get_attribute(self.name), 1)
        # self.unbind()


class IndexBuffer:
    def __init__(self, data: list, program, name: str, stepping=3) -> None:
        self.vbo = ogl.glGenBuffers(1)
        self.data = numpy.array(data, dtype="uint32")
        self.name = name
        self.program = program
        self.stepping = stepping
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
        self.program.set_attribute(self.name, self.stepping)
        # We dont unbind a GL_ELEMENT_ARRAY_BUFFER within the context of a vao
        # self.unbind()


class FrameBuffer:
    def __init__(self) -> None:
        self.fbo = ogl.glGenFramebuffers(1)

    def bind(self) -> None:
        ogl.glBindFramebuffer(ogl.GL_FRAMEBUFFER, self.fbo)

    def unbind(self) -> None:
        ogl.glBindFramebuffer(ogl.GL_FRAMEBUFFER, 0)

    def delete(self) -> None:
        ogl.glDeleteFramebuffers(1, self.fbo)


class Texture:
    def __init__(self, width, height, data):
        self.tbo = ogl.glGenTextures(1)
        self.width = width
        self.height = height
        self.data = data
        self._create_texture()

    @staticmethod
    def image_from_file(file_path):
        image = Image.open(file_path)
        data = numpy.array(list(image.getdata()), numpy.uint8)
        return Texture(image.size[0], image.size[1], data)

    def bind(self) -> None:
        ogl.glBindTexture(ogl.GL_TEXTURE_2D, self.tbo)

    def unbind(self) -> None:
        ogl.glBindTexture(ogl.GL_TEXTURE_2D, 0)

    def _create_texture(self) -> None:
        self.bind()
        ogl.glPixelStorei(ogl.GL_UNPACK_ALIGNMENT, 4)
        ogl.glTexParameteri(ogl.GL_TEXTURE_2D, ogl.GL_TEXTURE_WRAP_S, ogl.GL_REPEAT)
        ogl.glTexParameteri(ogl.GL_TEXTURE_2D, ogl.GL_TEXTURE_WRAP_T, ogl.GL_REPEAT)
        ogl.glTexParameteri(ogl.GL_TEXTURE_2D, ogl.GL_TEXTURE_MIN_FILTER, ogl.GL_LINEAR)
        ogl.glTexParameteri(ogl.GL_TEXTURE_2D, ogl.GL_TEXTURE_MAG_FILTER, ogl.GL_LINEAR)
        ogl.glTexImage2D(
            ogl.GL_TEXTURE_2D,
            0,
            ogl.GL_RGB,
            self.width,
            self.height,
            0,
            ogl.GL_RGB,
            ogl.GL_UNSIGNED_BYTE,
            self.data,
        )
        ogl.glGenerateMipmap(ogl.GL_TEXTURE_2D)
        self.unbind()
