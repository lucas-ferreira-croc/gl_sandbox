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

#include "tests/test.h"
#include "tests/test_clear_color.h"
#include "tests/test_texture2D.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

int main() 
{

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

	renderer renderer_;
	
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfwGL3_Init(window, true);

	ImGui::StyleColorsDark();


	test::test* currentTest = nullptr;
	test::test_menu* menu = new test::test_menu(currentTest);
	currentTest = menu;

	menu->register_test<test::test_clear_color>("Clear color");
	menu->register_test<test::test_texture2D>("Texture 2D");

	while(!glfwWindowShouldClose(window))
	{

		renderer_.clear();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		ImGui_ImplGlfwGL3_NewFrame();
		if(currentTest)
		{
			currentTest->on_update(0.0f);
			currentTest->on_render();
			
			ImGui::Begin("Test");
			if(currentTest != menu && ImGui::Button("<-"))
			{
				delete currentTest;
				currentTest = menu;
			}
			currentTest->on_imgui_render();
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	delete currentTest;
	if (currentTest != menu)
		delete menu;
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}