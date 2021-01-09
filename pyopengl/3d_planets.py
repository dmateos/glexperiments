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
        self.camera = camera.Camera(1280 / 768)

        self.program = shader.create_program(
            "renderer/shaders/camera_vert.gsl", "renderer/shaders/norm_frag.gsl"
        )
        self.program.use()
        self.program.set_uniform("projection", self.camera.mat_projection)

        self.instanced_program = shader.create_program(
            "renderer/shaders/instanced_camera_vert.gsl",
            "renderer/shaders/norm_frag.gsl",
        )
        self.instanced_program.use()
        self.instanced_program.set_uniform("projection", self.camera.mat_projection)

        self.cube0 = entities.Model(self.program, "assets/cube.obj", -5, 0, -5)
        self.cube1 = entities.Model(self.program, "assets/monkey.obj", 5, 0, -5)

        self.cubes = []

        for n in range(0, 10):
            self.cubes.extend([0.2 * n, 0.0, 0.2 * n])

        self.cube_group = entities.ModelGroup(
            self.instanced_program, "assets/cube.obj", self.cubes, 1, 1
        )

        texture = primitives.Texture.image_from_file("assets/container.jpg")
        texture.bind()

    def update(self):
        self.window.clear()
        self.program.use()
        self.program.set_uniform("camera", self.camera.mat_lookat)

        self.cube0.draw()
        self.cube1.draw()

        self.instanced_program.use()
        self.instanced_program.set_uniform("camera", self.camera.mat_lookat)

        self.cube_group.draw()

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
