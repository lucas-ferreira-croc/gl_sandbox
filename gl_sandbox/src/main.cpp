#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "renderer/renderer.h"
#include "buffers/vertex_buffer.h"
#include "buffers/index_buffer.h"
#include "vao/vertex_array.h"
#include "vao/vertex_buffer_layout.h"

#include "shader/shader.h"
#include "texture/texture.h"


int main() {

	GLFWwindow* window;

	if(!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1280, 1280, "gl_sandbox", NULL, NULL);

	if(!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		std::cout << "error initalizing glew" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	glBlendFunc(GL_SRC0_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	unsigned int index_data[6] = {
		0, 1, 2,
		1, 2, 3
	};

	float vertex_data[16] = {
	   -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 1.0f, 0.0f,
	   -0.5f,  0.5f, 0.0f, 1.0f,
		0.5f,  0.5f, 1.0f, 1.0f
	};

	vertex_array va;
	vertex_buffer vb(vertex_data, 4 * 4 * sizeof(float));

	vertex_buffer_layout layout;
	layout.push<float>(2);
	layout.push<float>(2);
	va.add_buffer(vb, layout);

	index_buffer ibo(index_data, 6);

	shader shader_program("res/shaders/basic_vertex.glsl", "res/shaders/basic_fragment.glsl");
	shader_program.bind();
	shader_program.set_uniform4f("u_Color", 0.7f, 0.0f, 0.7f, 1.0f);

	texture texture_("res/textures/crocao.png");
	texture_.bind();
	shader_program.set_uniform1i("u_Texture", 0);

	float r = 0.0f;
	float increment = 0.05f;

	va.unbind();
	vb.unbind();
	ibo.unbind();
	shader_program.unbind();

	renderer renderer_;
	while(!glfwWindowShouldClose(window))
	{
		renderer_.clear();

		if (r > 1.0f)
			increment = -0.05f;
		else if (r < 0.0f)
			increment = 0.05f;

		r += increment;	
		
		shader_program.bind();
		shader_program.set_uniform4f("u_Color", 0.7f, 0.0f, r, 1.0f);

		renderer_.draw(va, ibo, shader_program);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}