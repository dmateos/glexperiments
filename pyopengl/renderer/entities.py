import pyrr
from . import primitives

TEXTURE_COORDINATES = (1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0)


class ObjectFile(object):
    def __init__(self, path, debug=False):
        self.path = path
        self.verts = []
        self.normals = []
        self.vert_index = []
        self.norm_index = []
        self.debug = debug
        self.read()

    def read(self):
        data = ""
        with open(self.path, "r") as f:
            data = f.read()

        for line in data.split("\n"):
            if line.startswith("vn"):
                for vert in line.split(" ")[1:]:
                    self.normals.append(float(vert))
            elif line.startswith("v"):
                for vert in line.split(" ")[1:]:
                    self.verts.append(float(vert))
            elif line.startswith("f"):
                for vert in line.split(" ")[1:]:
                    self.vert_index.append(int(vert.split("//")[0]) - 1)
                    self.norm_index.append(int(vert.split("//")[1]) - 1)

        if self.debug:
            print("ObjReader")
            print(self.verts)
            print(self.vert_index)
            print(self.norm_index)

    def get_geometry(self):
        return [
            self.verts,
            self.vert_index,
            self.normals,
            self.norm_index,
        ]


class ModelInstance:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z


class Model(ModelInstance):
    def __init__(self, program, path, x, y, z):
        super().__init__(x, y, z)

        self.program = program
        self.vao = primitives.VertexState()
        self.scale_matrix = pyrr.Matrix44.from_scale([1, 1, 1])

        model_object = ObjectFile(path)
        self.geometry = model_object.get_geometry()

        with self.vao:
            primitives.VertexBuffer(self.geometry[0], self.program, "vp", 3)
            primitives.IndexBuffer(self.geometry[1], self.program, "vp")
            primitives.VertexBuffer(TEXTURE_COORDINATES, self.program, "tx", 2)
            # TODO Fix this
            primitives.VertexBuffer(self.geometry[0], self.program, "nm", 3)
            primitives.IndexBuffer(self.geometry[1], self.program, "nm")

    def draw(self):
        self.program.use()
        mat = pyrr.Matrix44.from_translation([self.x, self.y, self.z])

        self.program.set_uniform("translation", mat)
        self.program.set_uniform("scale", self.scale_matrix)

        with self.vao:
            self.vao.draw_indexed_elements(len(self.geometry[1]))


class ModelGroup:
    def __init__(self, program, path, models, width, height):
        self.program = program
        self.vao = primitives.VertexState()
        self.models = models
        self.scale_matrix = pyrr.Matrix44.from_scale([width, height, 1])
        model_object = ObjectFile(path)
        self.geometry = model_object.get_geometry()

        with self.vao:
            primitives.VertexBuffer(self.geometry[0], self.program, "vp", 3)
            primitives.IndexBuffer(self.geometry[1], self.program, "vp")
            primitives.VertexBuffer(TEXTURE_COORDINATES, self.program, "tx", 2)
            # TODO Fix this
            primitives.VertexBuffer(self.geometry[2], self.program, "nm", 3)
            primitives.IndexBuffer(self.geometry[1], self.program, "nm")

    def draw(self):
        self.program.use()
        verts = self.update_verticies()

        with self.vao:
            # TODO Update rather than new buffer each time?
            primitives.VertexBuffer(verts, self.program, "os", 3, True)
            self.program.set_uniform("scale", self.scale_matrix)
            self.vao.draw_instanced_indexed_elements(
                len(self.geometry[1]), int(len(verts) / 3)
            )

    def update_verticies(self):
        verts = []
        for n in self.models:
            verts.extend([n.x, n.y, n.z])
        return verts
