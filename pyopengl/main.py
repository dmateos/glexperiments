# import sys
from renderer import window, primitives, shader
import glfw
import pyrr


class Screen:
    def __init__(self):
        self.window = window.Window(1024, 768)
        self.window.run()

        self.window.add_key_callback(self.key_handler)
        self.window.reset_mouse_position()
        self.window.add_mouse_callback(self.mouse_handler)

        with open("renderer/shaders/basic_frag.gsl") as f:
            frag_data = f.read()
        with open("renderer/shaders/basic_vert.gsl") as f:
            vert_data = f.read()

        basic_frag = shader.Shader("fragment", frag_data)
        basic_vert = shader.Shader("vertex", vert_data)

        self.program = shader.Program()

        self.program.add_shader(basic_frag)
        self.program.add_shader(basic_vert)
        self.program.validate()
        self.program.compile()
        self.program.use()

        perspective = pyrr.Matrix44.perspective_projection(75.0, 1280 / 1024, 0.1, 1000.0)
        self.program.set_uniform("perspective", perspective)

        self.sprites = [
            primitives.Rectangle(self.program, 0, 0, 0.5, 0.5, [1.0, 0.0, 0.0]),
        ]


    def update(self):
        self.window.clear()

        for sprite in self.sprites:
            sprite.x += 0.001
            sprite.draw()

        self.window.swap()

    def key_handler(self, key, code, action, mods):
        if key == glfw.KEY_DOWN:
            pass
        elif key == glfw.KEY_UP:
            pass


    def mouse_handler(self, xpos, ypos):
        pass

if __name__ == "__main__":
    screen = Screen()

    while True:
        screen.update()