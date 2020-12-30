import OpenGL.GL as ogl
import enum
import typing


class ShaderType(enum.Enum):
    VERTEX_SHADER = 0
    FRAGMENT_SHADER = 1


class ShaderException(Exception):
    pass


class Shader:
    def __init__(self, shader_type: str, shader_code: str) -> None:
        self.shader_type = shader_type
        self.shader_code = shader_code
        self.shader = None

        self.compile()

    def compile(self) -> None:
        if self.shader_type == "vertex":
            self.shader = ogl.glCreateShader(ogl.GL_VERTEX_SHADER)
        elif self.shader_type == "fragment":
            self.shader = ogl.glCreateShader(ogl.GL_FRAGMENT_SHADER)
        else:
            raise ShaderException("not implemented")

        ogl.glShaderSource(self.shader, self.shader_code)
        ogl.glCompileShader(self.shader)

        result = ogl.glGetShaderiv(self.shader, ogl.GL_COMPILE_STATUS)

        if not result:
            print("could not compile shader {}".format(self.shader_type))

    def free(self) -> None:
        ogl.glDeleteShader(self.shader)

    @property
    def id(self):
        return self.shader


class Program:
    def __init__(self) -> None:
        self.program = ogl.glCreateProgram()
        self.shaders: typing.List[Shader] = []

    def add_shader(self, shader: Shader) -> None:
        self.shaders.append(shader)

    def compile(self) -> None:
        for shader in self.shaders:
            ogl.glAttachShader(self.program, shader.id)
        ogl.glLinkProgram(self.program)

    def use(self) -> None:
        ogl.glUseProgram(self.program)

    def unuse(self) -> None:
        ogl.glUseProgram(0)

    def validate(self) -> bool:
        ogl.glValidateProgram(self.program)
        validation = ogl.glGetProgramiv(self.program, ogl.GL_VALIDATE_STATUS)
        if validation == ogl.GL_FALSE:
            why = ogl.glGetProgramInfoLog(self.program)
            print("could not compile program: {0}".format(why))
            return False
        return True

    def get_attribute(self, name: str) -> str:
        attr = ogl.glGetAttribLocation(self.program, name)
        return attr

    def get_uniform(self, name: str) -> str:
        attr = ogl.glGetUniformLocation(self.program, name)
        return attr

    def set_uniform(self, name: str, value) -> None:
        ogl.glUniformMatrix4fv(self.get_uniform(name), 1, False, value)

    def set_attribute(self, name: str) -> None:
        # This call relies on the buffer being bound
        ogl.glEnableVertexAttribArray(self.get_attribute(name))
        ogl.glVertexAttribPointer(
            self.get_attribute(name), 3, ogl.GL_FLOAT, ogl.GL_FLOAT, 0, None
        )