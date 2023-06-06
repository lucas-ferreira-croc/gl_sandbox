#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static unsigned int compile_shader(const std::string& source, unsigned int type)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();

	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*) alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		
		std::cout << "Error compiling " << (type == GL_VERTEX_SHADER ? "vertex " : "fragment ") << " shader: ";
		std::cout << message << std::endl;
		
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int create_shader(const std::string& vertex_shader, const std::string& fragment_shader)
{
	 unsigned int program = glCreateProgram();
	 unsigned int vs = compile_shader(vertex_shader, GL_VERTEX_SHADER);
	 unsigned int fs = compile_shader(fragment_shader, GL_FRAGMENT_SHADER);

	 glAttachShader(program, vs);
	 glAttachShader(program, fs);
	 glLinkProgram(program);
	 glValidateProgram(program);

	 glDeleteShader(vs);
	 glDeleteShader(fs);

	 return program;
}

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

	if (glewInit() != GLEW_OK) {
		std::cout << "error initalizing glew" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	unsigned int vertex_buffer;
	float buffer_data [6] = { 
	   -0.5f, -0.5f,
		0.5f, -0.5f,
		0.0f,  0.5f
	};

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), buffer_data, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE , 2 * sizeof(float), 0);

	std::string vertex_shader =
		"#version 330 core \n"
		"\n"
		"layout(location = 0) in vec4 position; \n"
		"\n"
		"void main() \n"
		"{\n"
		"	gl_Position = position; \n"
		"}\n";

	std::string fragment_shader =
		"#version 330 core \n"
		"\n"
		"layout(location = 0) out vec4 color; \n"
		"\n"
		"void main() \n"
		"{\n"
		"	color = vec4(0.7, 0.0, 0.7, 1.0); \n"
		"}\n";

	unsigned int shader = create_shader(vertex_shader, fragment_shader);
	glUseProgram(shader);

	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	return 0;
}