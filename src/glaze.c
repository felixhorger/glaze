#include <stdlib.h>
#include <stdio.h>
#include "../include/glaze.h"


/*
	Default error callback

	Use by calling

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(gl_error_callback, 0);

*/
void GLAPIENTRY glErrorCallback(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* whatever
)
{
	// To improve this see https://www.khronos.org/opengl/wiki/Debug_Output
	fprintf(stderr,
		"OpenGL Error: type = 0x%x, severity = 0x%x, message = %s\n",
		type, severity, message
	);
	exit(EXIT_FAILURE);
}


void glHandleErrors(char* message)
{
	GLenum err = glGetError();
	do {
		if      (err == GL_NO_ERROR)                      return;
		else if (err == GL_INVALID_ENUM)                  fprintf(stderr, "OpenGL Error: invalid enum\n");
		else if (err == GL_INVALID_VALUE)                 fprintf(stderr, "OpenGL Error: invalid value\n");
		else if (err == GL_INVALID_OPERATION)             fprintf(stderr, "OpenGL Error: invalid operation\n");
		else if (err == GL_STACK_OVERFLOW)                fprintf(stderr, "OpenGL Error: stack overflow\n");
		else if (err == GL_STACK_UNDERFLOW)               fprintf(stderr, "OpenGL Error: stack underflow\n");
		else if (err == GL_OUT_OF_MEMORY)                 fprintf(stderr, "OpenGL Error: out of memory\n");
		else if (err == GL_INVALID_FRAMEBUFFER_OPERATION) fprintf(stderr, "OpenGL Error: invalid framebuffer operation\n");
		else if (err == GL_CONTEXT_LOST)                  fprintf(stderr, "OpenGL Error: context lost\n");
		else                                              fprintf(stderr, "OpenGL Error: unknown error\n");
	} while ((err = glGetError()) != GL_NO_ERROR);
	fprintf(stderr, "Error: %s\n", message);
	exit(EXIT_FAILURE);
}



/*
	Can check shader and program, user must provide the matching functions
	Only prints something if OpenGL set an info log.
	Otherwise behaves as all other OpenGL functions, setting a GLenum error
*/
void glCheckShaderProgram(
	GLuint program,
	GLenum pname,
	void (*glGetiv)(GLuint, GLenum, GLint*),
	void (*glGetInfoLog)(GLuint, GLsizei, GLsizei*, GLchar*),
	void (*glDelete)(GLuint),
	char* message
)
{
	GLuint result = GL_FALSE;
	glGetiv(program, pname, &result);
	if (result == GL_TRUE) return;
	// if result == GL_FALSE _and_ if program or pname is invalid

	GLint length = 0; // includes null char
	glGetiv(program, GL_INFO_LOG_LENGTH, &length);
	if (length > 0) {
		char* error = malloc(length);
		glGetInfoLog(program, length, NULL, error);
		glDelete(program);
		fprintf(stderr, "OpenGL Error: %s\nError: %s\n", error, message);
	}
	glHandleErrors("encountered while checking shader/program");

	exit(EXIT_FAILURE);
}



GLuint glMakeShader(GLenum type, const char* const* source)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, source, NULL);
	glCompileShader(shader);
	glHandleErrors("compiling shader");

	glCheckShaderProgram(
		shader,
		GL_COMPILE_STATUS,
		glGetShaderiv, glGetShaderInfoLog, glDeleteShader,
		"could not compile shader"
	);

	return shader;
}

GLuint glMakeProgram(GLuint *shaders, size_t n)
{
	GLuint program = glCreateProgram();

	char message[24];
	for (size_t i = 0; i < n; i++) {
		glAttachShader(program, shaders[i]);
		snprintf(message, sizeof(message), "attaching shader %u", i);
		glHandleErrors(message);
	}

	glLinkProgram(program);
	glCheckShaderProgram(
		program,
		GL_LINK_STATUS,
		glGetProgramiv, glGetProgramInfoLog, glDeleteProgram,
		"could not link program"
	);

	glValidateProgram(program);
	glCheckShaderProgram(
		program,
		GL_VALIDATE_STATUS,
		glGetProgramiv, glGetProgramInfoLog, glDeleteProgram,
		"invalid program"
	);

	return program;
}



//int main(int argc, char** argv)
//{
//	return 0;
//}
