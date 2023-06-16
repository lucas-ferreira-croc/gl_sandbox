#pragma once

#include <GL/glew.h>
#include "../buffers/index_buffer.h"
#include "../vao/vertex_array.h"
#include "../shader/shader.h"

#define ASSERT(x) if ((!x)) __debugbreak();
#define GLCall(x) gl_clear_error();\
	x;\
	ASSERT(gl_log_call(#x, __FILE__, __LINE__))

void gl_clear_error();

bool  gl_log_call(const char* function, const char* file, int line);

class renderer
{
public:
	void clear();
	void draw(const vertex_array& vao, const index_buffer& ibo, const shader& shader_) const;


};