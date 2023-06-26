#pragma once

#include "test.h"

#include "../buffers/index_buffer.h"
#include "../vao/vertex_buffer_layout.h"
#include "../texture/texture.h"
#include <memory>
#include "glm/glm.hpp"


namespace test
{
	class test_texture2D : public test
	{
	private:
		glm::vec3 m_translationA, m_translationB;
		glm::mat4 m_proj, m_view;

		std::unique_ptr<vertex_buffer> m_vb;
		std::unique_ptr<vertex_array> m_vao;
		std::unique_ptr<index_buffer> m_ibo;
		std::unique_ptr<shader> m_shader;
		std::unique_ptr<texture> m_texture;
	public:
		test_texture2D();
		~test_texture2D();

		virtual void on_update(float delta_time) override;
		virtual void on_render() override;
		virtual void on_imgui_render() override;
	};
}

