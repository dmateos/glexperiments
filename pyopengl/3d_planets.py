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

        self.monkey = entities.Model(self.program, "assets/monkey.obj", -3.0, 0, 10)

        self.cubes = []

        for n in range(0, 50):
            for nn in range(0, 50):
                for nnn in range(0, 50):
                    self.cubes.extend([2.0 * n, 2.0 * nn, -2.0 * nnn])

        self.monkeys = []
        for n in range(0, 10):
            for nn in range(0, 10):
                for nnn in range(0, 10):
                    self.monkeys.extend([-2.0 * n + -8.0, 2.0 * nn, -2.0 * nnn])

        self.cube_group = entities.ModelGroup(
            self.instanced_program, "assets/cube.obj", self.cubes, 1, 1
        )

        self.monkey_group = entities.ModelGroup(
            self.instanced_program, "assets/monkey.obj", self.monkeys, 1, 1
        )

        texture = primitives.Texture.image_from_file("assets/container.jpg")
        texture.bind()

    def update(self):
        self.window.clear()
        self.program.use()
        self.program.set_uniform("camera", self.camera.mat_lookat)

        self.instanced_program.use()
        self.instanced_program.set_uniform("camera", self.camera.mat_lookat)

        self.monkey.draw()

        self.cube_group.draw()
        self.monkey_group.draw()

        self.window.swap()

    def ok(self):
        return self.window.ok()

    def key_handler(self, key, code, action, mods):
        if key == glfw.KEY_S:
            self.camera.move_backwards()
        elif key == glfw.KEY_W:
            self.camera.move_forward()
        elif key == glfw.KEY_A:
            self.camera.rotate_left()
        elif key == glfw.KEY_D:
            self.camera.rotate_right()
        elif key == glfw.KEY_E:
            self.window.toggle_wireframe()

    def mouse_handler(self, xpos, ypos):
        pass


def run():
    screen = Screen()
    while screen.ok():
        screen.update()


if __name__ == "__main__":
    run()
