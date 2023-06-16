#pragma once
#include "../buffers/vertex_buffer.h"
#include "vertex_buffer_layout.h"

class vertex_array
{
private:
	unsigned int m_renderer_id;
public:

	vertex_array();
	~vertex_array();

	void add_buffer(const vertex_buffer& vb, const vertex_buffer_layout& layout);
	void bind() const;
	void unbind() const;
};

