import pyrr


class Camera3D:
    def __init__(self):
        self.eye = [1, 1, 0]
        self._perspective = pyrr.Matrix44.perspective_projection(
            75.0, 1280 / 1024, 0.1, 1000.0
        )

    def perspective_matrix(self):
        return self._perspective

    def camera_matrix(self):
        return pyrr.matrix44.create_look_at(self.eye, (1, 1, -1), (0, 1, 0))
