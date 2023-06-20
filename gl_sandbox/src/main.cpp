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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

int main() {

	GLFWwindow* window;

	if(!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1280, 720, "gl_sandbox", NULL, NULL);

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
	    100.0f,  100.0f, 0.0f, 0.0f,
		200.0f,  100.0f, 1.0f, 0.0f,
	    100.0f,  200.0f, 0.0f, 1.0f,
		200.0f,  200.0f, 1.0f, 1.0f
	};

	vertex_array va;
	vertex_buffer vb(vertex_data, 4 * 4 * sizeof(float));

	vertex_buffer_layout layout;
	layout.push<float>(2);
	layout.push<float>(2);
	va.add_buffer(vb, layout);

	index_buffer ibo(index_data, 6);

	glm::mat4 proj = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));

	shader shader_program("res/shaders/basic_vertex.glsl", "res/shaders/basic_fragment.glsl");
	shader_program.bind();
	shader_program.set_uniform4f("u_Color", 0.7f, 0.0f, 0.7f, 1.0f);
	
	texture texture_("res/textures/crocao.png");
	texture_.bind();
	shader_program.set_uniform1i("u_Texture", 0);

	float r = 0.0f;
	float increment = 0.05f;
	glm::vec3 translation(200, 200, 0);

	va.unbind();
	vb.unbind();
	ibo.unbind();
	shader_program.unbind();

	renderer renderer_;
	
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfwGL3_Init(window, true);

	ImGui::StyleColorsDark();

	while(!glfwWindowShouldClose(window))
	{
		ImGui_ImplGlfwGL3_NewFrame();

		renderer_.clear();

		if (r > 1.0f)
			increment = -0.05f;
		else if (r < 0.0f)
			increment = 0.05f;

		r += increment;	

		glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
		glm::mat4 mvp = proj * view * model;
		
		shader_program.bind();
		shader_program.set_uniform4f("u_Color", 0.7f, 0.0f, r, 1.0f);
		shader_program.set_uniform_mat4f("u_MVP", mvp);

		renderer_.draw(va, ibo, shader_program);

		{
			ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 1280.0f);      
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}