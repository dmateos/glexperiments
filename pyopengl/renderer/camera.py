import pyrr
import numpy as np


class Camera:
    def __init__(self) -> None:
        self.position = [0.0, 0.0, 0.0]
        self._perspective = pyrr.Matrix44.perspective_projection(
            75.0, 1280 / 1024, 0.1, 1000.0
        )

        self.angle = 0.0
        self.orientation = pyrr.Quaternion()
        self.forward_vector = [0.0, 0.0, 1.0, 1.0]

    def perspective(self) -> pyrr.Matrix44:
        return self._perspective

    def matrix(self) -> pyrr.Matrix44:
        trans = pyrr.Matrix44.from_translation(self.position)
        return self.orientation * trans.inverse

    def move(self, x: float, y: float, z: float):
        self.position[0] += x
        self.position[1] += y
        self.position[2] += z

    def pan(self, degrees: float):
        self.angle += degrees
        self.orientation = pyrr.Quaternion.from_y_rotation(np.pi * self.angle)

    def forward(self):
        self.position = self.position * (
            (self.orientation * self.forward_vector) * 0.10
        )


class Camera2d:
    def __init__(self) -> None:
        pass


class Camera3d:
    def __init__(self) -> None:
        pass
