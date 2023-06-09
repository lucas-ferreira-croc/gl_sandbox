#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define ASSERT(x) if ((!x)) __debugbreak();
#define GLCall(x) gl_clear_error();\
	x;\
	ASSERT(gl_log_call(#x, __FILE__, __LINE__))
	
static void gl_clear_error()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool  gl_log_call(const char* function, const char* file, int line)
{
	while(GLenum error = glGetError())
	{
		std::cout << "[OpenGL error]: ( " << error << " ): " << function << " " << file << ": " << line << std::endl;
		return false;
	}
	return true;
}

struct shader_program_source
{
	std::string vertex;
	std::string fragment;
};

static std::string read_shader_from_file(const std::string& file)
{
	std::ifstream stream(file);
	std::stringstream buffer;

	buffer << stream.rdbuf();
	stream.close();
	
	std::string source_code = buffer.str();
	
	return source_code;
}

static shader_program_source parse_shaders(const std::string& vertex, const std::string& fragment)
{
	
	return { read_shader_from_file(vertex), read_shader_from_file(fragment) };
}

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

	unsigned int vertex_buffer;
	
	
	unsigned int index_buffer;
	unsigned int index_data[6] = {
		0, 1, 2,
		1, 2, 3
	};

	float buffer_data[8] = {
	   -0.5f, -0.5f,
		0.5f, -0.5f,
	   -0.5f,  0.5f,
		0.5f,  0.5f
	};

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(float), index_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE , 2 * sizeof(float), 0);


	shader_program_source shader_source = parse_shaders("res/shaders/basic_vertex.glsl", "res/shaders/basic_fragment.glsl");
	unsigned int shader = create_shader(shader_source.vertex, shader_source.fragment);
	glUseProgram(shader);

	GLCall(int location = glGetUniformLocation(shader, "u_Color"));
	glUniform4f(location, 0.7f, 0.0f, 0.7f, 1.0f);

	float r = 0.0f;
	float increment = 0.05f;

	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		
		if (r > 1.0f)
			increment = -0.05f;
		else if (r < 0.0f)
			increment = 0.05f;

		r += increment;
		glUniform4f(location, r, 0.0f, 0.7f, 1.0f);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
	glDeleteProgram(shader);

	return 0;
}