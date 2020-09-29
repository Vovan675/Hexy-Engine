#include "pch.h"
#include "Renderer.h"
#include "Renderer2D.h"

namespace Hexy
{
	std::shared_ptr<Shader> Renderer::m_lineShader;

	void Renderer::Init()
	{
		m_lineShader = ShaderLibrary::Use("assets/shaders/LineShader.vert", "assets/shaders/LineShader.frag");
	}

	void Renderer::Begin()
	{

	}

	void Renderer::End()
	{
		
	}

	void Renderer::DrawMesh(std::shared_ptr<Mesh> mesh, glm::mat4 transform)
	{
		//mesh->GetVertexArray()->Draw();
		mesh->GetVertexArray()->Bind();

		auto materials = mesh->GetMaterials();
		for (Submesh submesh : mesh->GetSubmeshes()) 
		{
			auto material = materials[submesh.materialIndex];

			material->GetShader()->Bind();
			material->GetShader()->SetMat4("u_modelMatrix", transform * submesh.transform);

			material->Bind();
			glDrawElementsBaseVertex(GL_TRIANGLES, submesh.vertexCount, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * submesh.baseIndex), submesh.baseVertex);
		}
	}

	void Renderer::DrawLine(glm::vec3 a, glm::vec3 b)
	{
		auto va = VertexArray::Create(RenderType::LINES);
		const float vertices[3 * 2] = {
				a.x, a.y, a.z,
				b.x, b.y, b.z
		};

		auto vbo = VertexBuffer::Create(vertices, sizeof(vertices));
		vbo->SetLayout({
			{ElementType::Float3}, //POSITION
			});
		unsigned int indices[2] = {
				0, 1,
		};
		auto ibo = IndexBuffer::Create(indices, 2);
		va->SetVertexBuffer(vbo);
		va->SetIndexBuffer(ibo);

		m_lineShader->Bind();
		va->Draw();
	}

	void Renderer::DrawAABB(const AABB& aabb, glm::mat4& transform)
	{
		glm::vec4 points[8] = {
			transform * glm::vec4(aabb.min.x, aabb.min.y, aabb.max.z, 1),
			transform * glm::vec4(aabb.min.x, aabb.max.y, aabb.max.z, 1),
			transform * glm::vec4(aabb.max.x, aabb.max.y, aabb.max.z, 1),
			transform * glm::vec4(aabb.max.x, aabb.min.y, aabb.max.z, 1),

			transform * glm::vec4(aabb.min.x, aabb.min.y, aabb.min.z, 1),
			transform * glm::vec4(aabb.min.x, aabb.max.y, aabb.min.z, 1),
			transform * glm::vec4(aabb.max.x, aabb.max.y, aabb.min.z, 1),
			transform * glm::vec4(aabb.max.x, aabb.min.y, aabb.min.z, 1),
		};
		
		//DrawLine(aabb.min, aabb.max);
		for (int i = 0; i < 4; i++) 
		{
			DrawLine(points[i], points[(i + 1) % 4]);
		}
		for (int i = 0; i < 4; i++) 
		{
			DrawLine(points[i+4], points[((i + 1) % 4) + 4]);
		}
		for (int i = 0; i < 4; i++) 
		{
			DrawLine(points[i], points[i + 4]);
		}
	}
}