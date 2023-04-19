#include "pch.h"
#include "Material.h"

namespace Hexy 
{
	Material::Material(std::shared_ptr<Shader> shader)
	{
		this->shader = shader;
		albedo = TextureLibrary::LoadBlankTexture();
		metalness = TextureLibrary::LoadBlankTexture();
		roughness = TextureLibrary::LoadBlankTexture();
		normal = TextureLibrary::LoadBlankTexture();
		ao = TextureLibrary::LoadBlankTexture();

		m_uniformsData = new unsigned char[1024];

		Set("u_useAlbedoTexture", 0.0f);
		Set("u_AlbedoTexture", albedo);
		Set("u_Albedo", {0, 0, 0});

		Set("u_useMetalnessTexture", 0.0f);
		Set("u_MetalnessTexture", metalness);
		Set("u_Metalness", 0.0f);

		Set("u_useRoughnessTexture", 0.0f);
		Set("u_RoughnessTexture", roughness);
		Set("u_Roughness", 0.0f);

		Set("u_useNormalTexture", 0.0f);
		Set("u_NormalTexture", normal);

		Set("u_useAOTexture", 0.0f);
		Set("u_AOTexture", ao);
	}

	Material::~Material()
	{
		delete[] m_uniformsData;
	}

	void Material::Bind()
	{
		//Set basic uniforms
		shader->Bind();
		for (auto uniform : m_uniforms)
		{
			if (uniform.type == ShaderUniform::Int)
			{
				shader->SetInt(uniform.name, *(int*)&m_uniformsData[uniform.offset]);
			}
			else if (uniform.type == ShaderUniform::Float)
			{
				shader->SetFloat(uniform.name, *(float*)&m_uniformsData[uniform.offset]);
			}
			else if (uniform.type == ShaderUniform::Vec2)
			{
				shader->SetVec2(uniform.name, *(glm::vec2*)&m_uniformsData[uniform.offset]);
			}
			else if (uniform.type == ShaderUniform::Vec3)
			{
				shader->SetVec3(uniform.name, *(glm::vec3*)&m_uniformsData[uniform.offset]);
			}
			else if (uniform.type == ShaderUniform::Vec4)
			{
				shader->SetVec4(uniform.name, *(glm::vec4*)&m_uniformsData[uniform.offset]);
			}
		}

		//Binding textures
		for (int i = 0; i < m_textures.size(); i++) 
		{
			m_textures[i]->Bind(i);
		}
	}

	void Material::Set(const std::string& name, int value)
	{
		for (auto uniform : m_uniforms) 
		{
			if (uniform.name == name)
			{
				memcpy(m_uniformsData + uniform.offset, &value, sizeof(int));
				return;
			}
		}

		unsigned int offset = 0;
		if (!m_uniforms.empty())
		{
			offset = m_uniforms.back().offset + m_uniforms.back().size;
		}
		m_uniforms.push_back({ name, ShaderUniform::Type::Int, offset });
		memcpy(m_uniformsData + offset, &value, sizeof(int));
	}

	void Material::Set(const std::string& name, float value)
	{
		for (auto uniform : m_uniforms) 
		{
			if (uniform.name == name) 
			{
				memcpy(m_uniformsData + uniform.offset, &value, sizeof(float));
				return;
			}
		}
		
		unsigned int offset = 0;
		if (!m_uniforms.empty()) 
		{
			offset = m_uniforms.back().offset + m_uniforms.back().size;
		}
		m_uniforms.push_back({ name, ShaderUniform::Type::Float, offset });
		memcpy(m_uniformsData + offset, &value, sizeof(float));
	}

	void Material::Set(const std::string& name, const glm::vec3& value)
	{
		for (auto uniform : m_uniforms) 
		{
			if (uniform.name == name) 
			{
				memcpy(m_uniformsData + uniform.offset, &value, sizeof(float) * 3);
				return;
			}
		}

		unsigned int offset = 0;
		if (!m_uniforms.empty()) 
		{
			offset = m_uniforms.back().offset + m_uniforms.back().size;
		}
		m_uniforms.push_back({ name, ShaderUniform::Type::Vec3, offset });
		memcpy(m_uniformsData + offset, &value, sizeof(float)*3);
	}


	void Material::Set(const std::string& name, const std::shared_ptr<Texture2D>& texture)
	{
		//TODO: FIX SET OF SAME TEXTURE
		int slot = m_textures.size();

		for (auto uniform : m_uniforms)
		{
			if (uniform.name == name) 
			{
				slot = *(int*)&m_uniformsData[uniform.offset];
				break;
			}
		}

		Set(name, slot);
		if (m_textures.size() <= slot)
		{
			m_textures.resize(slot + 1);
		}
		m_textures[slot] = texture;
	}

	std::shared_ptr<Texture2D> Material::Get(const std::string& name)
	{
		for (auto uniform : m_uniforms) 
		{
			if (uniform.name == name)
			{
				return m_textures[*(int*)&m_uniformsData[uniform.offset]];
			}
		}
		return nullptr;
	}

	float Material::GetFloat(const std::string& name)
	{
		for (auto uniform : m_uniforms)
		{
			if (uniform.type == ShaderUniform::Float && uniform.name == name)
			{
				return *(float*)&m_uniformsData[uniform.offset];

			}
		}
		return 0;
	}


	/////////////////MATERIAL INSTANCE/////////////////

	MaterialInstance::MaterialInstance(std::shared_ptr<Material> material) : m_material(material)
	{

	}

	void MaterialInstance::Bind()
	{

	}

	void MaterialInstance::Set(const std::string& name, float value)
	{

	}
}