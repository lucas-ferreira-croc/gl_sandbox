#include <GLFW/glfw3.h>

int main() {
	GLFWwindow* window;

	if(!glfwInit())
	{
		return -1;
	}

	window = glfwCreateWindow(1280, 720, "gl_sandbox", NULL, NULL);

	if(!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		
		//TO DO: remove boiler plate legacy openGL code
		glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.5f, -0.5f);
		glVertex2f(0.0f, 0.5f);
		glEnd();
		
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	return 0;
}