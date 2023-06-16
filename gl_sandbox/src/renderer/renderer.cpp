#pragma once
#include "renderer.h"
#include "GL/glew.h"
#include <iostream>

void gl_clear_error()
{
	while (glGetError() != GL_NO_ERROR);
}

bool  gl_log_call(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL error]: ( " << error << " ): " << function << " " << file << ": " << line << std::endl;
		return false;
	}
	return true;
}

void renderer::clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void renderer::draw(const vertex_array& vao, const index_buffer& ibo, const shader& shader_) const
{
	vao.bind();
	shader_.bind();
	ibo.bind();

	glDrawElements(GL_TRIANGLES, ibo.get_count(), GL_UNSIGNED_INT, nullptr);
}
