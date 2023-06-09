#pragma once

#include <string>
#include <unordered_map>

#include "../vendor/glm/glm.hpp"

class shader
{
private:
	std::string m_vertex_filepath;
	std::string m_fragment_filepath;
	unsigned int m_renderer_id;

	std::unordered_map<std::string, int> m_uniform_location_cache;

public:
	shader(const std::string& vertex_file, const std::string& fragment_file);
	~shader();

	void bind() const;
	void unbind() const;

	void set_uniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void set_uniform_mat4f(const std::string& name, glm::mat4 value);
	void set_uniform1i(const std::string& name, int value);
		
private:	
	unsigned int create_shader(const std::string& vertex_shader, const std::string& fragment_shader);
	unsigned int compile_shader(const std::string& source, unsigned int type);
	int get_uniform_location(const std::string& name);
};
