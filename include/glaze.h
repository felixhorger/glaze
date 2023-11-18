#include "../include/glad.h"

void GLAPIENTRY glErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* whatever);
void glHandleErrors(char* message);
void glCheckShaderProgram(GLuint program, GLenum pname, void (*glGetiv)(GLuint, GLenum, GLint*), void (*glGetInfoLog)(GLuint, GLsizei, GLsizei*, GLchar*), void (*glDelete)(GLuint), char* message);
GLuint glMakeShader(GLenum type, const char* const* source);
GLuint glMakeProgram(GLuint *shaders, size_t n);

