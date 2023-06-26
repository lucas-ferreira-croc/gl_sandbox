#include "test_texture2D.h"

#include "../buffers/index_buffer.h"
#include "../vao/vertex_array.h"
#include "../shader/shader.h"


#include "../renderer/renderer.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"


namespace test
{
	test_texture2D::test_texture2D()
		: m_translationA(200, 200, 0), m_translationB(500, 500, 0),
		  m_proj(glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f)), m_view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
		  
	{
		float vertex_data[16] = {
		-50.0f,  -50.0f, 0.0f, 0.0f,
		 50.0f,  -50.0f, 1.0f, 0.0f,
		-50.0f,   50.0f, 0.0f, 1.0f,
		 50.0f,   50.0f, 1.0f, 1.0f
		};

		unsigned int index_data[6] = {
			0, 1, 2,
			1, 2, 3
		};

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC0_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_vao = std::make_unique<vertex_array>();
		m_shader = std::make_unique<shader>("res/shaders/basic_vertex.glsl", "res/shaders/basic_fragment.glsl");
		m_ibo = std::make_unique<index_buffer>(index_data, 6);

		m_vb = std::make_unique<vertex_buffer> (vertex_data, 4 * 4 * sizeof(float));

		vertex_buffer_layout layout;
		layout.push<float>(2);
		layout.push<float>(2);
		m_vao->add_buffer(*m_vb, layout);
	
		m_shader->bind();
		m_shader->set_uniform4f("u_Color", 0.7f, 0.0f, 0.7f, 1.0f);

		m_texture = std::make_unique<texture>("res/textures/crocao.png");

		m_shader->set_uniform1i("u_Texture", 0);

		m_vao->unbind();
		m_vb->unbind();
		m_ibo->unbind();
		m_shader->unbind();
	}

	test_texture2D ::~test_texture2D()
	{
	}

	void test_texture2D::on_update(float delta_time)
	{
	}

	void test_texture2D::on_render()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		renderer renderer_;

		m_texture->bind();
		 
		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationA);
		glm::mat4 mvp = m_proj * m_view * model;
		

		{
			m_shader->bind();
			m_shader->set_uniform_mat4f("u_MVP", mvp);
			renderer_.draw(*m_vao, *m_ibo, *m_shader);

		}

		model = glm::translate(glm::mat4(1.0f), m_translationB);
		mvp = m_proj * m_view * model;

		{
			m_shader->bind();
			m_shader->set_uniform_mat4f("u_MVP", mvp);
			renderer_.draw(*m_vao, *m_ibo, *m_shader);
		}
		
	}

	void test_texture2D::on_imgui_render()
	{
		ImGui::SliderFloat3("TranslationA", &m_translationA.x, 0.0f, 1280.0f);
		ImGui::SliderFloat3("TranslationB", &m_translationB.x, 0.0f, 1280.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}