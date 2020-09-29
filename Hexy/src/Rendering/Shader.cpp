#include "pch.h"
#include "Shader.h"
#include "DebugInfo.h"

namespace Hexy 
{
	Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) : m_vertexShaderName(vertexShaderPath), m_fragmentShaderName(fragmentShaderPath), m_type(VertexFragment) {
		m_program = glCreateProgram();
		m_vs = glCreateShader(GL_VERTEX_SHADER);
		m_fs = glCreateShader(GL_FRAGMENT_SHADER);

		compileShaders();
		DebugInfo::ShaderLoaded++;
	}

	Shader::Shader(const char* computeShaderPath) : m_computeShaderName(computeShaderPath), m_type(Compute)
	{
		m_program = glCreateProgram();
		m_cs = glCreateShader(GL_COMPUTE_SHADER);

		compileShaders();
		DebugInfo::ShaderLoaded++;
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_program);
	}

	void Shader::Bind()
	{
		glUseProgram(m_program);
	}

	void Shader::Unbind()
	{
		glUseProgram(0);
	}

	void Shader::Reload()
	{
		compileShaders();
	}

	std::shared_ptr<Shader> Shader::Create(const char* vertexShaderPath, const char* fragmentShaderPath)
	{
		return std::make_shared<Shader>(vertexShaderPath, fragmentShaderPath);
	}

	std::shared_ptr<Shader> Shader::Create(const char* computeShaderPath)
	{
		return std::make_shared<Shader>(computeShaderPath);
	}

	void Shader::compileShaders() 
	{
		if (m_type == ShaderType::VertexFragment) 
		{
			std::string vsFile = getSourceFromFile(m_vertexShaderName.c_str());
			std::string fsFile = getSourceFromFile(m_fragmentShaderName.c_str());
			const char* vsSource = vsFile.c_str();
			const char* fsSource = fsFile.c_str();

			glShaderSource(m_vs, 1, &vsSource, NULL);
			glShaderSource(m_fs, 1, &fsSource, NULL);

			char buf[512];
			int status;

			glCompileShader(m_vs);
			glGetShaderiv(m_vs, GL_COMPILE_STATUS, &status);
			if (!status)
			{
				glGetShaderInfoLog(m_vs, sizeof(buf), nullptr, buf);
				std::cout << "Vertex Shader Error: " << buf;
			}
			glAttachShader(m_program, m_vs);


			glCompileShader(m_fs);
			glGetShaderiv(m_fs, GL_COMPILE_STATUS, &status);
			if (!status)
			{
				glGetShaderInfoLog(m_fs, sizeof(buf), nullptr, buf);
				std::cout << "Fragment Shader Error: " << buf;
			}
			glAttachShader(m_program, m_fs);


			glLinkProgram(m_program);
			glGetProgramiv(m_program, GL_LINK_STATUS, &status);
			if (!status)
			{
				glGetProgramInfoLog(m_program, 512, NULL, buf);
				std::cout << buf << std::endl;
			}

			glDeleteShader(m_vs);
			glDeleteShader(m_fs);
		}
		else if (m_type == ShaderType::Compute)
		{
			std::string csFile = getSourceFromFile(m_computeShaderName.c_str());
			const char* csSource = csFile.c_str();
			glShaderSource(m_cs, 1, &csSource, NULL);

			char buf[512];
			int status;

			glCompileShader(m_cs);
			glGetShaderiv(m_cs, GL_COMPILE_STATUS, &status);
			if (!status)
			{
				glGetShaderInfoLog(m_cs, sizeof(buf), nullptr, buf);
				std::cout << "Compute Shader Error: " << buf;
			}
			glAttachShader(m_program, m_cs);


			glLinkProgram(m_program);
			glGetProgramiv(m_program, GL_LINK_STATUS, &status);
			if (!status) 
			{
				glGetProgramInfoLog(m_program, 512, NULL, buf);
				std::cout << buf << std::endl;
			}

			glDeleteShader(m_cs);
		}
	}

	std::string Shader::getSourceFromFile(const char* path)
	{
		std::stringstream source;
		std::ifstream file;
		file.open(path);
		source << file.rdbuf();
		file.close();
		return source.str();
	}

	void Shader::SetFloat(const std::string& name, float value)
	{
		auto loc = glGetUniformLocation(m_program, name.c_str());
		glUniform1f(loc, value);
	}

	void Shader::SetInt(const std::string& name, int value)
	{
		glUniform1i(glGetUniformLocation(m_program, name.c_str()), value);
	}

	void Shader::SetVec2(const std::string& name, glm::vec2 values) 
	{
		glUniform2f(glGetUniformLocation(m_program, name.c_str()), values.x, values.y);
	}

	void Shader::SetVec3(const std::string& name, glm::vec3 values) 
	{
		glUniform3f(glGetUniformLocation(m_program, name.c_str()), values.x, values.y, values.z);
	}

	void Shader::SetVec4(const std::string& name, glm::vec4 values)
	{
		glUniform4f(glGetUniformLocation(m_program, name.c_str()), values.x, values.y, values.z, values.w);
	}

	void Shader::SetMat4(const std::string& name, glm::mat4 matrix)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_program, name.c_str()), 1, false, glm::value_ptr(matrix));
	}

	/////////////Shader Library////////////

	std::vector<std::shared_ptr<Shader>> ShaderLibrary::m_shaders;

	std::shared_ptr<Shader> ShaderLibrary::Use(const char* vertexShaderPath, const char* fragmentShaderPath)
	{
		for (auto shader : m_shaders)
		{
			if (shader->GetVertexShaderName() == vertexShaderPath && shader->GetFragmentShaderName() == fragmentShaderPath) 
			{
				return shader;
			}
		}
		std::shared_ptr<Shader> shader = Shader::Create(vertexShaderPath, fragmentShaderPath);
		m_shaders.push_back(shader);
		return shader;
	}

	std::shared_ptr<Shader> ShaderLibrary::Use(const char* computeShaderPath)
	{
		for (auto shader : m_shaders)
		{
			if (shader->GetComputeShaderName() == computeShaderPath) 
			{
				return shader;
			}
		}
		std::shared_ptr<Shader> shader = Shader::Create(computeShaderPath);
		m_shaders.push_back(shader);
		return shader;
	}
}