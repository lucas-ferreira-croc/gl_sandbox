#pragma once
#include <vector>
#include <functional>
#include <string>
#include <iostream>

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


	class test_menu : public test
	{
	private:
		std::vector<std::pair<std::string, std::function<test* ()>>> m_tests;
		test*& m_current_test;

	public:
		test_menu(test*& current_test);
		~test_menu();
		
		template<typename T>
		void register_test(const std::string& name)
		{
			std::cout << "Registering test " << name << std::endl;

			m_tests.push_back(std::make_pair(name, []() {
				return new T();
			}));
		}

		virtual void on_imgui_render() override;
	};
}