#pragma once


namespace test
{
	class test
	{
	public:
		test() {}
		virtual ~test(){}

		virtual void on_update(float delta_time){}
		virtual void on_render() {}
		virtual void on_imgui_render() {}
	};
}