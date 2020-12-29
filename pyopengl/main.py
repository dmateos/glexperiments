# import sys
from renderer import window, primitives, shader
import glfw
import pyrr

main_window = window.Window(1024, 768)
main_window.run()

with open("renderer/shaders/basic_frag.gsl") as f:
    frag_data = f.read()
with open("renderer/shaders/basic_vert.gsl") as f:
    vert_data = f.read()

basic_frag = shader.Shader("fragment", frag_data)
basic_vert = shader.Shader("vertex", vert_data)

program = shader.Program()
program.add_shader(basic_frag)
program.add_shader(basic_vert)
program.validate()
program.compile()
program.use()

perspective = pyrr.Matrix44.perspective_projection(75.0, 1280 / 1024, 0.1, 1000.0)
program.set_uniform("perspective", perspective)

sprites = [
    primitives.Rectangle(program, 0, 0, 0.5, 0.5, [1.0, 0.0, 0.0]),
]


def key_handler(key, code, action, mods):
    if key == glfw.KEY_W:
        pass
    elif key == glfw.KEY_S:
        pass
    elif key == glfw.KEY_A:
        pass
    elif key == glfw.KEY_D:
        pass


def mouse_handler(xpos, ypos):
    main_window.reset_mouse_position()


main_window.add_key_callback(key_handler)
main_window.add_mouse_callback(mouse_handler)

while main_window.ok():
    main_window.clear()

    for sprite in sprites:
        sprite.x += 0.001
        sprite.draw()

    main_window.swap()
