from renderer import window, shader, sprites


SCREEN_SIZE = (1024, 768)


class Screen:
    def __init__(self):
        self.window = window.Window(SCREEN_SIZE[0], SCREEN_SIZE[1])
        self.window.run()

        self.window.add_key_callback(self.key_handler)
        self.window.reset_mouse_position()
        self.window.add_mouse_callback(self.mouse_handler)

        self.program = shader.create_program(
            shader.BASIC_VERT_SHADER, shader.BASIC_FRAG_SHADER
        )

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
