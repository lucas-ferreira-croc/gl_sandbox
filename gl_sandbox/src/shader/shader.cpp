#include "shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "GL/glew.h"
struct shader_program_source
{
	std::string vertex;
	std::string fragment;

	shader_program_source(const std::string& vertex_path, const std::string& fragment_path)
		:vertex(read_shader_from_file(vertex_path)), fragment(read_shader_from_file(fragment_path))
	{
		
	}

	std::string read_shader_from_file(const std::string& file)
	{
		std::ifstream stream(file);
		std::stringstream buffer;

		buffer << stream.rdbuf();
		stream.close();

		std::string source_code = buffer.str();

		return source_code;
	}
};

shader::shader(const std::string& vertex_file, const std::string& fragment_file)
	:m_vertex_filepath(vertex_file), m_fragment_filepath(fragment_file), m_renderer_id(0)
{
	shader_program_source source(m_vertex_filepath, m_fragment_filepath);
	create_shader(source.vertex, source.fragment);
}

shader::~shader()
{
	glDeleteProgram(m_renderer_id);
}

void shader::bind() const
{
	glUseProgram(m_renderer_id);
}

void shader::unbind() const 
{
	glUseProgram(0);
}

void shader::set_uniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	int location = get_uniform_location(name);
	glUniform4f(location, v0, v1, v2, v3);
}

void shader::set_uniform_mat4f(const std::string& name, glm::mat4 value)
{
	int location = get_uniform_location(name);
	glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

void shader::set_uniform1i(const std::string& name, int value)
{
	int location = get_uniform_location(name);
	glUniform1i(location, value);
}



unsigned int shader::create_shader(const std::string& vertex_shader, const std::string& fragment_shader)
{
	 m_renderer_id = glCreateProgram();
	unsigned int vs = compile_shader(vertex_shader, GL_VERTEX_SHADER);
	unsigned int fs = compile_shader(fragment_shader, GL_FRAGMENT_SHADER);

	glAttachShader(m_renderer_id, vs);
	glAttachShader(m_renderer_id, fs);
	glLinkProgram(m_renderer_id);
	glValidateProgram(m_renderer_id);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return m_renderer_id;
}



unsigned int shader::compile_shader(const std::string& source, unsigned int type)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();

	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Error compiling " << (type == GL_VERTEX_SHADER ? "vertex " : "fragment ") << " shader: ";
		std::cout << message << std::endl;

		glDeleteShader(id);
		return 0;
	}

	return id;
}


int shader::get_uniform_location(const std::string& name)
{
	if(m_uniform_location_cache.find(name) != m_uniform_location_cache.end())
		return m_uniform_location_cache[name];
	
	int location = glGetUniformLocation(m_renderer_id, name.c_str());
	return m_uniform_location_cache[name] = location;
	return location;
}
