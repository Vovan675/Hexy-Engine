#pragma once
#include "Application.h"
#include "stb_image.h"
#include <unordered_map>
#include "MeshFactory.h"
#include "Shader.h"

namespace Hexy
{
	class Texture2D
	{
	private:
		unsigned int m_texture = 0;
		std::string m_texturePath;
	public:
		Texture2D(std::string path);
		Texture2D();
		~Texture2D();

		void Bind(unsigned int slot);

		void SetTexture(const std::string& path);
		unsigned int GetTexture() { return m_texture; }

		static std::shared_ptr<Texture2D> Create(const std::string& path);
		static std::shared_ptr<Texture2D> Create();
	};

	/////////////Cube Texture///////////////

	class TextureCube 
	{
	private:
		unsigned int m_texture;
		unsigned int m_irradiance;
		unsigned int m_prefilter;
		unsigned int m_brdf;

		unsigned int m_captureRenderbuffer;
		unsigned int m_captureFramebuffer;

		unsigned int m_width;
		unsigned int m_height;
	public:
		TextureCube(std::string path);
		~TextureCube();

		void Bind(unsigned int slot);

		void SetTexture(const std::string& path);

		unsigned int GetTexture() { return m_texture; }
		unsigned int GetIrradiance() { return m_irradiance; }
		unsigned int GetPrefilter() { return m_prefilter; }
		unsigned int GetBRDF() { return m_brdf; }

		unsigned int GetMipLevels();

		static std::shared_ptr<TextureCube> Create(const std::string& path);
	private:
		void GenerateIrradiance();
		void GeneratePrefilter();
		void GenerateBRDF();
	};

	/////////////Texture Library////////////

	class TextureLibrary 
	{
	private:
		static std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_textures;
		static std::shared_ptr<Texture2D> m_blank;
	public:
		static std::shared_ptr<Texture2D> Load(const std::string& path);
		static std::shared_ptr<Texture2D> LoadBlankTexture();
	};

}