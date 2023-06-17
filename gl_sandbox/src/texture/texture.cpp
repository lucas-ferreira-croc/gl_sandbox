#include "texture.h"
#include "../vendor/stb_image/stb_image.h"

texture::texture(const std::string& path)
	: m_filepath(path), m_local_buffer(nullptr), m_renderer_id(0), m_bpp(0)
{
	
	stbi_set_flip_vertically_on_load(1);
	m_local_buffer = stbi_load(path.c_str(), &m_width, &m_height, &m_bpp, 4);

	glGenTextures(1, &m_renderer_id);
	glBindTexture(GL_TEXTURE_2D, m_renderer_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_local_buffer);
	unbind();

	if (m_local_buffer)
		stbi_image_free(m_local_buffer);
}

texture::~texture()
{
	glDeleteTextures(1, &m_renderer_id);
}

void texture::bind(unsigned int slot /* = 0*/) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_renderer_id);
}

void texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
