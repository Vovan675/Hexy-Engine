#include "pch.h"
#include "Texture.h"
#include "DebugInfo.h"

namespace Hexy 
{
	Texture2D::Texture2D(std::string path)
	{
		SetTexture(path);
		DebugInfo::TextureLoaded++;
	}

	Texture2D::Texture2D() : m_texturePath("blank")
	{
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		unsigned char* image = new unsigned char[3]{ 255, 255, 255 };
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		DebugInfo::TextureLoaded++;
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_texture);
	}

	void Texture2D::Bind(unsigned int slot)
	{
		glBindTextureUnit(slot, m_texture);
	}

	void Texture2D::SetTexture(const std::string& path) 
	{
		/*
		if (TextureLibrary::Exists(m_texturePath)) { //Maybe do it in another way(Set texture MUST set another texture to already exist instance)
			auto node = TextureLibrary::m_textures.extract(m_texturePath); 
			node.key() = path;
			TextureLibrary::m_textures.insert(std::move(node));
		}*/
		m_texturePath = path;
		stbi_set_flip_vertically_on_load(1);
		int texwidth, texheight, channels;
		unsigned char* image = stbi_load(path.c_str(), &texwidth, &texheight, &channels, 4);
		
		if (image == NULL) 
		{
			std::cout << stbi_failure_reason();
			return;
		}
		/*
		unsigned int format;
		unsigned int internalFormat;
		if (channels == 1) {
			format = GL_RED;
			internalFormat = GL_RED;
		}
		if (channels == 4) {
			format = GL_RGBA;
			internalFormat = GL_RGBA;
		}
		else if (channels == 3) {
			format = GL_RGB;
			internalFormat = GL_RGB;
		}*/

		if (m_texture)
		{
			glDeleteTextures(1, &m_texture);
		}
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texwidth, texheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(image);
	}



	std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path)
	{
		return std::make_shared<Texture2D>(path);
	}
	std::shared_ptr<Texture2D> Texture2D::Create()
	{
		return std::make_shared<Texture2D>();
	}

	/////////////Cube Texture///////////////

	//Creates a cube texture from single texture
	TextureCube::TextureCube(std::string path)
	{
		glGenFramebuffers(1, &m_captureFramebuffer);
		glGenRenderbuffers(1, &m_captureRenderbuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_captureFramebuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_captureRenderbuffer);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_captureRenderbuffer);
		SetTexture(path);
	}

	TextureCube::~TextureCube()
	{

	}

	void TextureCube::Bind(unsigned int slot)
	{
		glBindTextureUnit(slot, m_texture);
	}


	const glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	const glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	//Only HDR textures currently supported!
	void TextureCube::SetTexture(const std::string& path)
	{
		//Need to create render buffer and render then i need to get its attachment and simply set it to texture
		stbi_set_flip_vertically_on_load(1);
		int texwidth, texheight, channels;
		float* image = stbi_loadf(path.c_str(), &texwidth, &texheight, &channels, 3);
		
		if (image == NULL) 
		{
			std::cout << stbi_failure_reason();
			return;
		}

		unsigned int equiTexture; //Its simple texture, to be converted to cube texture
		glGenTextures(1, &equiTexture);
		glBindTexture(GL_TEXTURE_2D, equiTexture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, texwidth, texheight, 0, GL_RGB, GL_FLOAT, image);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		 
		glBindTexture(GL_TEXTURE_2D, 0);
		
		stbi_image_free(image);
		
		//For now all cube maps will be 2048 by 2048 in size

		m_width = 2048;
		m_height = 2048;
		//Reserve cubemap
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

		glTextureStorage2D(m_texture, GetMipLevels(), GL_RGB16F, m_width, m_height);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		//Convert the texture to cubemap

		glBindFramebuffer(GL_FRAMEBUFFER, m_captureFramebuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_width, m_height);

		auto shader = Shader::Create("assets/shaders/texture_to_cube.vert", "assets/shaders/texture_to_cube.frag");

		auto cube = MeshFactory::Cube();
		shader->Bind();
		glBindTextureUnit(0, equiTexture);



		int oldViewport[4];
		glGetIntegerv(GL_VIEWPORT, oldViewport);
		glViewport(0, 0, m_width, m_height);

		shader->SetMat4("u_projection", captureProjection);
		glBindFramebuffer(GL_FRAMEBUFFER, m_captureFramebuffer);
		for (int i = 0; i < 6; i++) {
			shader->SetMat4("u_view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_texture, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			cube->Draw();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		glViewport(oldViewport[0], oldViewport[1], oldViewport[2], oldViewport[3]);

		GenerateIrradiance();
		GeneratePrefilter();
		GenerateBRDF();
		//glDelete...
	}

	void TextureCube::GenerateIrradiance()
	{
		//Reserve irradiance map (32 x 32 pixels)
		glGenTextures(1, &m_irradiance);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_irradiance);

		glTextureStorage2D(m_irradiance, 6, GL_RGB16F, 32, 32);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Generate irradiance map
		glBindFramebuffer(GL_FRAMEBUFFER, m_captureFramebuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

		auto shader = Shader::Create("assets/shaders/texture_to_cube.vert", "assets/shaders/irradiance.frag");

		auto cube = MeshFactory::Cube();
		shader->Bind();
		glBindTextureUnit(0, m_texture); //Bind cubemap

		int oldViewport[4];
		glGetIntegerv(GL_VIEWPORT, oldViewport);
		glViewport(0, 0, 32, 32);

		shader->SetMat4("u_projection", captureProjection);
		glBindFramebuffer(GL_FRAMEBUFFER, m_captureFramebuffer);
		for (int i = 0; i < 6; i++)
		{
			shader->SetMat4("u_view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_irradiance, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			cube->Draw();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glBindTexture(GL_TEXTURE_CUBE_MAP, m_irradiance);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		glViewport(oldViewport[0], oldViewport[1], oldViewport[2], oldViewport[3]);
	}

	void TextureCube::GeneratePrefilter()
	{
		//Reserve prefilter map
		glGenTextures(1, &m_prefilter);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_prefilter);

		glTextureStorage2D(m_prefilter, GetMipLevels(), GL_RGB16F, m_width, m_height);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Generate prefilter map

		auto shader = Shader::Create("assets/shaders/texture_to_cube.vert", "assets/shaders/prefilter.frag");

		auto cube = MeshFactory::Cube();
		shader->Bind();
		glBindTextureUnit(0, m_texture); //Bind cubemap

		int oldViewport[4];
		glGetIntegerv(GL_VIEWPORT, oldViewport);

		shader->SetMat4("u_projection", captureProjection);

		glBindFramebuffer(GL_FRAMEBUFFER, m_captureFramebuffer);
		const int maxMipLevels = GetMipLevels();
		for (int mip = 0; mip < maxMipLevels; ++mip) {
			unsigned int mipWidth = m_width * std::pow(0.5, mip);
			unsigned int mipHeight = m_height * std::pow(0.5, mip);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
			glViewport(0, 0, mipWidth, mipHeight);

			shader->SetFloat("u_roughness", (float)mip / (float)(maxMipLevels - 1));
			for (int i = 0; i < 6; i++) {
				shader->SetMat4("u_view", captureViews[i]);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_prefilter, mip);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				cube->Draw();
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		m_texture = m_prefilter;
		glViewport(oldViewport[0], oldViewport[1], oldViewport[2], oldViewport[3]);
	}

	void TextureCube::GenerateBRDF()
	{
		//Reserve BRDF map
		glGenTextures(1, &m_brdf);
		glBindTexture(GL_TEXTURE_2D, m_brdf);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 512, 512, 0, GL_RG, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Generate BRDF map
		glBindFramebuffer(GL_FRAMEBUFFER, m_captureFramebuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);

		auto shader = Shader::Create("assets/shaders/BRDFConvolution.vert", "assets/shaders/BRDFConvolution.frag");

		auto quad = MeshFactory::FullScreenQuad();
		shader->Bind();
		glBindTextureUnit(0, m_texture); //Bind cubemap

		int oldViewport[4];
		glGetIntegerv(GL_VIEWPORT, oldViewport);
		glViewport(0, 0, 512, 512);

		glBindFramebuffer(GL_FRAMEBUFFER, m_captureFramebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_brdf, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		quad->Draw();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(oldViewport[0], oldViewport[1], oldViewport[2], oldViewport[3]);
	}

	unsigned int TextureCube::GetMipLevels()
	{
		unsigned int levels = 1;
		while ((m_width | m_height) >> levels)
			levels++;
		return levels;
	}

	std::shared_ptr<TextureCube> TextureCube::Create(const std::string& path)
	{
		return std::make_shared<TextureCube>(path);
	}

	/////////////Texture Library////////////

	std::unordered_map<std::string, std::shared_ptr<Texture2D>> TextureLibrary::m_textures;
	std::shared_ptr<Texture2D> TextureLibrary::m_blank;

	std::shared_ptr<Texture2D> TextureLibrary::Load(const std::string& path)
	{
		if (m_textures.find(path) != m_textures.end()) 
		{
			return m_textures[path];
		}
		std::shared_ptr<Texture2D> texture = Texture2D::Create(path);
		m_textures[path] = texture;
		return texture;
	}

	std::shared_ptr<Texture2D> TextureLibrary::LoadBlankTexture()
	{
		if (!m_blank) 
		{
			m_blank = Texture2D::Create();
		}
		return m_blank;
	}
}