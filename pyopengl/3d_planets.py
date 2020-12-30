from renderer import window, shader, sprites


SCREEN_SIZE = (1024, 768)


class Screen:
    def __init__(self):
        self.window = window.Window(SCREEN_SIZE[0], SCREEN_SIZE[1])
        self.window.run()

        self.window.add_key_callback(self.key_handler)
        self.window.reset_mouse_position()
        self.window.add_mouse_callback(self.mouse_handler)

        with open("renderer/shaders/basic_frag.gsl") as f:
            frag_data = f.read()
        with open("renderer/shaders/instanced_vert.gsl") as f:
            vert_data = f.read()

        basic_frag = shader.Shader("fragment", frag_data)
        basic_vert = shader.Shader("vertex", vert_data)

        self.program = shader.Program()

        self.program.add_shader(basic_frag)
        self.program.add_shader(basic_vert)
        self.program.validate()
        self.program.compile()
        self.program.use()

    def update(self):
        pass

    def ok(self):
        return self.window.ok()

    def key_handler(self, key, code, action, mods):
        pass

    def mouse_handler(self, xpos, ypos):
        pass


def run():
    screen = Screen()
    while screen.ok():
        screen.update()


if __name__ == "__main__":
    run()
