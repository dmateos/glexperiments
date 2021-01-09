from renderer import window, shader, camera, entities, primitives
import glfw


SCREEN_SIZE = (1024, 768)


class Screen:
    def __init__(self):
        self.window = window.Window(SCREEN_SIZE[0], SCREEN_SIZE[1])
        self.window.run(True)

        self.window.add_key_callback(self.key_handler)
        self.window.reset_mouse_position()
        self.window.add_mouse_callback(self.mouse_handler)

        self.program = shader.create_program(
            "renderer/shaders/camera_vert.gsl", shader.BASIC_FRAG_SHADER
        )

        self.camera = camera.Camera(1280 / 768)
        self.program.set_uniform("projection", self.camera.mat_projection)

        self.cube = entities.Cube(self.program, 1, 0, -5)
        self.cube2 = entities.Model(self.program, "assets/cube.obj", -5, 0, -5)
        self.cube3 = entities.Model(self.program, "assets/monkey.obj", 5, 0, -5)

        texture = primitives.Texture.image_from_file("assets/container.jpg")
        texture.bind()

    def update(self):
        self.window.clear()
        self.program.set_uniform("camera", self.camera.mat_lookat)
        self.cube.draw()
        self.cube2.draw()
        self.cube3.draw()
        self.window.swap()

    def ok(self):
        return self.window.ok()

    def key_handler(self, key, code, action, mods):
        if key == glfw.KEY_DOWN:
            self.camera.move_backwards()
        elif key == glfw.KEY_UP:
            self.camera.move_forward()
        elif key == glfw.KEY_LEFT:
            self.camera.rotate_left()
        elif key == glfw.KEY_RIGHT:
            self.camera.rotate_right()

    def mouse_handler(self, xpos, ypos):
        pass


def run():
    screen = Screen()
    while screen.ok():
        screen.update()


if __name__ == "__main__":
    run()
