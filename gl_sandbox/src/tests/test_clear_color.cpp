#include "test_clear_color.h"

#include "../renderer/renderer.h"
#include "imgui/imgui.h"


namespace test
{
	test_clear_color::test_clear_color()
		: m_color { 0.5f, 0.0f, 0.5f, 1.0f }
	{
	}

	test_clear_color::~test_clear_color()
	{
	}

	void test_clear_color::on_update(float delta_time)
	{
	}

	void test_clear_color::on_render()
	{
		GLCall(glClearColor(m_color[0], m_color[1], m_color[2], m_color[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void test_clear_color::on_imgui_render()
	{
		ImGui::ColorEdit4("Clear Color", m_color);
	}
}