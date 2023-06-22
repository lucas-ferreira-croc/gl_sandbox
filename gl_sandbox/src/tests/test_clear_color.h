#pragma once

#include "test.h"

namespace test
{
	class test_clear_color : public test
	{
	private:
		float m_color[4];

	public:
		test_clear_color();
		~test_clear_color();

		virtual void on_update(float delta_time) override;
		virtual void on_render() override;
		virtual void on_imgui_render() override;
	};
}

