#include "test.h"
#include "imgui/imgui.h"

namespace test
{
	test_menu::test_menu(test*& current_test)
		: m_current_test(current_test)
	{
	}

	test_menu::~test_menu()
	{
	}
	
	void test_menu::on_imgui_render()
	{
		for (auto& test : m_tests)
		{
			if(ImGui::Button(test.first.c_str()))
			{
				m_current_test = test.second();
			}
		}
	}
}