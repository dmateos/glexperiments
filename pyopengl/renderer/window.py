import glfw
import OpenGL.GL as ogl


def cb_dbg_msg(source, msg_type, msg_id, severity, length, raw, user):
    msg = raw[0:length]
    print("debug", source, msg_type, msg_id, severity, msg)


class Window:
    def __init__(self, x: int, y: int) -> None:
        self.x = x
        self.y = y
        self.window = None
        self._key_callbacks = []
        self._mouse_callbacks = []
        self.framecount = 0
        self.previous_time = 0

    def run(self, depth=False) -> None:
        if not glfw.init():
            return

        self.previous_time = glfw.get_time()

        glfw.window_hint(glfw.CONTEXT_VERSION_MAJOR, 3)
        glfw.window_hint(glfw.CONTEXT_VERSION_MINOR, 2)
        glfw.window_hint(glfw.OPENGL_PROFILE, glfw.OPENGL_CORE_PROFILE)
        glfw.window_hint(glfw.OPENGL_FORWARD_COMPAT, True)

        self.window = glfw.create_window(self.x, self.y, "Window", None, None)
        glfw.set_key_callback(self.window, self._key_event)
        glfw.set_input_mode(self.window, glfw.STICKY_KEYS, ogl.GL_TRUE)

        glfw.set_cursor_pos_callback(self.window, self._mouse_event)
        glfw.set_input_mode(self.window, glfw.CURSOR, glfw.CURSOR_DISABLED)

        glfw.make_context_current(self.window)

        ogl.glClearColor(0, 1, 0, 0)
        # ogl.glPolygonMode(ogl.GL_FRONT_AND_BACK, ogl.GL_LINE)

        if depth:
            ogl.glEnable(ogl.GL_DEPTH_TEST)
            ogl.glEnable(ogl.GL_CULL_FACE)
            ogl.glDepthFunc(ogl.GL_LESS)

    def set_clear_color(self, r, g, b):
        ogl.glClearColor(r, g, b, 0)

    def swap(self) -> None:
        glfw.swap_buffers(self.window)

    def poll(self) -> None:
        glfw.poll_events()

    def clear(self) -> None:
        ogl.glClear(ogl.GL_COLOR_BUFFER_BIT | ogl.GL_DEPTH_BUFFER_BIT)

    def ok(self, poll=True) -> bool:
        if poll:
            self.poll()
            self.get_fps()
        return not glfw.window_should_close(self.window)

    def add_key_callback(self, callback) -> None:
        self._key_callbacks.append(callback)

    def _key_event(self, window, key, scancode, action, mods) -> None:
        for cb in self._key_callbacks:
            cb(key, scancode, action, mods)

    def add_mouse_callback(self, callback) -> None:
        self._mouse_callbacks.append(callback)

    def _mouse_event(self, thing, xpos, ypos) -> None:
        for cb in self._mouse_callbacks:
            cb(xpos, ypos)

    def reset_mouse_position(self) -> None:
        glfw.set_cursor_pos(self.window, 0, 0)

    def get_fps(self) -> None:
        cur_time = glfw.get_time()
        self.framecount += 1

        if cur_time - self.previous_time >= 1.0:
            glfw.set_window_title(self.window, str(self.framecount))
            self.framecount = 0
            self.previous_time = cur_time
