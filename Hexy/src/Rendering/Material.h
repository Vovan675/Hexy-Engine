#pragma once
#include "ShaderUniform.h"
#include "Shader.h"
#include "Texture.h"

namespace Hexy 
{
	class Material
	{
	private:
		//In future it would hold Uniform buffer which textures will be added by 'Set' function with name and value arguments
		std::shared_ptr<Shader> shader = nullptr;
		std::shared_ptr<Texture2D> albedo = nullptr;
		std::shared_ptr<Texture2D> metalness = nullptr;
		std::shared_ptr<Texture2D> roughness = nullptr;
		std::shared_ptr<Texture2D> normal = nullptr;
		std::shared_ptr<Texture2D> ao = nullptr;

		std::vector<ShaderUniform> m_uniforms;
		unsigned char* m_uniformsData;

		std::vector<std::shared_ptr<Texture2D>> m_textures;
	public:
		Material(std::shared_ptr<Shader> shader);
		~Material();

		void Bind();

		void Set(const std::string& name, int value);
		void Set(const std::string& name, float value);
		void Set(const std::string& name, const glm::vec3& value);

		void Set(const std::string& name, const std::shared_ptr<Texture2D>& texture);

		std::shared_ptr<Texture2D> Get(const std::string& name);

		const std::shared_ptr<Shader>& GetShader() { return shader; }
	};

	class MaterialInstance 
	{
	private:
		std::shared_ptr<Material> m_material;

	public:
		MaterialInstance(std::shared_ptr<Material> material);

		void Bind();

		const std::shared_ptr<Shader>& GetShader() { return m_material->GetShader(); }

		void Set(const std::string& name, float value);
	};
}