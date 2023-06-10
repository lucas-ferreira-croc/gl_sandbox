#include <GL/glew.h>

#include "index_buffer.h"
#include "../renderer/renderer.h"

index_buffer::index_buffer(const unsigned int* data, unsigned int count)
	: m_count(count)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	glGenBuffers(1, &m_renderer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

index_buffer::~index_buffer()
{
	glDeleteBuffers(1, &m_renderer_id);
}

void index_buffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
}

void index_buffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
