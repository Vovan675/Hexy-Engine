#pragma once
#include "Application.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Hexy 
{
	class Shader 
	{
	private:
		unsigned int m_program, m_vs, m_fs, m_cs;
		std::string m_vertexShaderName;
		std::string m_fragmentShaderName;
		std::string m_computeShaderName;

		enum ShaderType
		{
			VertexFragment,
			Compute
		} m_type;
	public:
		Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
		Shader(const char* computeShaderPath);
		~Shader();

		void Bind();
		void Unbind();
		void Reload();

		inline std::string GetVertexShaderName() { return m_vertexShaderName; }
		inline std::string GetFragmentShaderName() { return m_fragmentShaderName; }
		inline std::string GetComputeShaderName() { return m_computeShaderName; }

		static std::shared_ptr<Shader> Create(const char* vertexShaderPath, const char* fragmentShaderPath);
		static std::shared_ptr<Shader> Create(const char* computeShaderPath);

		void SetFloat(const std::string& name, float value);
		void SetInt(const std::string& name, int value);

		void SetVec2(const std::string& name, glm::vec2 values);
		void SetVec3(const std::string& name, glm::vec3 values);
		void SetVec4(const std::string& name, glm::vec4 values);

		void SetMat4(const std::string& name, glm::mat4);
	private:
		void compileShaders();
		std::string getSourceFromFile(const char* path);
	};

	/////////////Shader Library////////////

	class ShaderLibrary
	{
	private:
		static std::vector<std::shared_ptr<Shader>> m_shaders;
	public:
		static std::shared_ptr<Shader> Use(const char* vertexShaderPath, const char* fragmentShaderPath);
		static std::shared_ptr<Shader> Use(const char* computeShaderPath);
	};
}