#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../include/glaze.h" // TODO need to register this somehow, how?
#include <GLFW/glfw3.h>

void error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error: %s\n", description);
	exit(EXIT_FAILURE);
	return;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	return;
}


int main(int argc, char* argv[]) {

	if (!glfwInit()) {
		printf("Error: could not initialise GLFW");
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_SAMPLES, 4);
	glfwSetErrorCallback(error_callback);

	int width = 500, height = 300;
	GLFWwindow* window = glfwCreateWindow(width, height, "", NULL, NULL);
	if (!window) {
		printf("Error: could not open window");
		exit(EXIT_FAILURE);
	}
	//glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0); // TODO: turn on?
	//printf("%s\n", glGetString(GL_VERSION));

	gladLoadGL();
	// if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) ?
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(glErrorCallback, 0);


	const char* vertex_shader_source = "\
		#version 450 core		\n\
		layout (location = 0) in vec2 in_position;\n\
		out vec2 position;				\n\
		void main() {					\n\
			gl_Position = vec4(in_position, 0.0, 1.0);\n\
			float zoom = 0.0003; \n\
			vec2 shift = vec2(-1.3941, 0.0042); \n\
			position[0] = shift[0] + zoom * 1.8 * in_position[0];\n\
			position[1] = shift[1] + zoom * in_position[1];		\n\
		}								\n\
	";
	const char* fragment_shader_source = "\
		#version 450 core										\n\
		in vec2 position;										\n\
		out vec4 colour;										\n\
		void main(void) {										\n\
			dvec2 c = position;								\n\
			dvec2 z = vec2(0, 0);									\n\
			dvec2 z0;											\n\
			int n = 0;										\n\
			int m = 8000; \n\
			while (sqrt(z[0]*z[0] + z[1]*z[1]) / sqrt(c[0] * c[0] + c[1] * c[1]) <= 5 && n <= m) {					\n\
				z0 = z;											\n\
				z[0] = z0[0] * z0[0] - z0[1] * z0[1] + c[0];	\n\
				z[1] = 2 * z0[0] * z0[1] + c[1];				\n\
				n += 1;											\n\
			}													\n\
			n -= 1; \n\
			if (n == m) { \n\
				colour = vec4(0, 0, 0, 1);				\n\
			} \n\
			else { \n\
				float f = min(10 * float(n) / float(m), 1); \n\
				colour = vec4(1-pow(max(f-0.5, 0), 1) / pow(1 - 0.5, 1), 0, 0, 1);				\n\
			} \n\
		}														\n\
	";

	GLuint program, shaders[2];
	shaders[0] = glMakeShader(GL_VERTEX_SHADER, &vertex_shader_source);
	shaders[1] = glMakeShader(GL_FRAGMENT_SHADER, &fragment_shader_source);
	program = glMakeProgram(shaders, 2);
	glUseProgram(program);

	const float view[] = {
		-1.0f, -1.0f,
		 1.0f, -1.0f,
		 1.0f,  1.0f,
		-1.0f,  1.0f
	};

	GLuint buffers[2];
	glGenBuffers(2, &buffers[0]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(view), view, GL_STATIC_DRAW);
	GLuint vertex_array;
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(vertex_array);

	glClearColor(0, 0, 0, 1);
	glDisable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();

		//while (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS) {
		//	glfwPollEvents();
		//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		//		glfwSetWindowShouldClose(window, 1);
		//		break;
		//	}
		//	usleep(10000);
		//}
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

