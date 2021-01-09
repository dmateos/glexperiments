import pyrr
from . import primitives

TEXTURE_COORDINATES = (1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0)


class Cube:
    def __init__(self, program, x, y, z, color=[1, 1, 1]):
        self.x = x
        self.y = y
        self.z = z
        self.color = color

        self.program = program
        self.vao = primitives.VertexState()
        self.scale_matrix = pyrr.Matrix44.from_scale([1, 1, 1])
        self.geometry = pyrr.geometry.create_cube(
            scale=[1.0, 1.0, 1.0],
            st=False,
            rgba=False,
            dtype="float32",
            type="triangles",
        )

        with self.vao:
            primitives.VertexBuffer(self.geometry[0], program, "vp", 3)
            primitives.IndexBuffer(self.geometry[1], program, "vp")
            primitives.VertexBuffer(self.color, program, "c", 3)
            primitives.VertexBuffer(TEXTURE_COORDINATES, program, "tx", 2)

    def draw(self):
        self.program.use()
        mat = pyrr.Matrix44.from_translation([self.x, self.y, self.z])

        self.program.set_uniform("translation", mat)
        self.program.set_uniform("scale", self.scale_matrix)

        with self.vao:
            self.vao.draw_indexed_elements(len(self.geometry[1]))
