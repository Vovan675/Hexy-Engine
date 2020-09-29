#include "pch.h"
#include "Renderer2D.h"

namespace Hexy
{
	std::shared_ptr<VertexArray> Renderer2D::quadVA;
	std::shared_ptr<Shader> Renderer2D::basicShader;
	std::shared_ptr<Texture2D> Renderer2D::basicTexture;

	std::shared_ptr<VertexArray> Renderer2D::fullscreenQuadVA;

	void Renderer2D::Init()
	{
		basicShader = ShaderLibrary::Use("assets/shaders/FlatShader.vert", "assets/shaders/FlatShader.frag");

		const float quadVertices[(2 + 2) * 4] = {
			-0.5,  -0.5,	0, 0,
			 0.5,   0.5,	1, 1,
			 0.5,  -0.5,	1, 0,
			-0.5,   0.5,	0, 1
		};
		quadVA = Hexy::VertexArray::Create();

		std::shared_ptr<Hexy::VertexBuffer> vbo = Hexy::VertexBuffer::Create(quadVertices, sizeof(quadVertices));
		vbo->SetLayout({
			{Hexy::ElementType::Float2}, //POSITION
			{Hexy::ElementType::Float2}, //UV
			});
		quadVA->SetVertexBuffer(vbo);

		unsigned int indices[3 * 2] = {
			0, 1, 2,
			3, 1, 0
		};

		std::shared_ptr<Hexy::IndexBuffer> ibo = Hexy::IndexBuffer::Create(indices, 6);
		quadVA->SetIndexBuffer(ibo);

		basicTexture = TextureLibrary::LoadBlankTexture();

		fullscreenQuadVA = MeshFactory::FullScreenQuad();
	}

	void Renderer2D::Begin(CameraComponent* camera)
	{
		basicShader->Bind();
		basicShader->SetMat4("u_cameraMatrix", camera->GetMatrix());
	}

	void Renderer2D::End()
	{

	}

	void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 scale, glm::vec3 color)
	{
		basicShader->Bind();
		basicShader->SetVec3("u_color", color);
		glm::mat4 modelMatrix = glm::scale(glm::translate(glm::mat4(1), { position.x, position.y, 0 }), { scale.x, scale.y, 0});
		basicShader->SetMat4("u_modelMatrix", modelMatrix);
		basicTexture->Bind(0);
		quadVA->Draw();
	}

	void Renderer2D::DrawQuad(glm::vec2 position, std::shared_ptr<Texture2D>& texture, glm::vec2 scale, glm::vec3 color)
	{
		basicShader->Bind();
		basicShader->SetVec3("u_color", color);
		glm::mat4 modelMatrix = glm::scale(glm::translate(glm::mat4(1), { position.x, position.y, 0 }), { scale.x, scale.y, 0 });
		basicShader->SetMat4("u_modelMatrix", modelMatrix);
		texture->Bind(0);
		quadVA->Draw();
	}

	void Renderer2D::DrawQuad(glm::mat4 transform, std::shared_ptr<Texture2D>& texture, glm::vec2 scale, glm::vec3 color)
	{
		//TODO: UNUSED PARAMETER
		basicShader->Bind();
		basicShader->SetVec3("u_color", color);
		basicShader->SetMat4("u_modelMatrix", transform);
		texture->Bind(0);
		quadVA->Draw();
	}

	void Renderer2D::DrawQuad(glm::mat4 transform, glm::vec3 color)
	{
		basicShader->Bind();
		basicShader->SetVec3("u_color", color);
		basicShader->SetMat4("u_modelMatrix", transform);
		basicTexture->Bind(0);
		quadVA->Draw();
	}

	void Renderer2D::DrawFulscreenQuad()
	{
		fullscreenQuadVA->Draw();
	}
}