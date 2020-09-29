#include "pch.h"
#include "MeshFactory.h"

namespace Hexy
{
	std::shared_ptr<VertexArray> MeshFactory::Plane()
	{
		const float vertices[(2 + 2) * 4] = {
			-0.5f, -0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, 1.0f, 0.0f,
		};
		auto va = Hexy::VertexArray::Create();

		auto vbo = Hexy::VertexBuffer::Create(vertices, sizeof(vertices));
		vbo->SetLayout({
			{Hexy::ElementType::Float2}, //POSITION
			{Hexy::ElementType::Float2}, //UV
			});
		va->SetVertexBuffer(vbo);
		unsigned int indices[3 * 2] = {
				0, 1, 2,
				2, 3, 0
		};
		auto ibo = Hexy::IndexBuffer::Create(indices, 6);
		va->SetIndexBuffer(ibo);

		return va;
	}

	std::shared_ptr<VertexArray> MeshFactory::Triangle()
	{
		const float vertices[(2 + 2) * 3] = {
			-0.5f, -0.5f,  0.0f, 0.0f,
			 0.0f,  0.5f,  0.5f, 1.0f,
			 0.5f, -0.5f,  1.0f, 0.0f,
		};
		auto va = Hexy::VertexArray::Create();

		auto vbo = Hexy::VertexBuffer::Create(vertices, sizeof(vertices));
		vbo->SetLayout({
			{Hexy::ElementType::Float2}, //POSITION
			{Hexy::ElementType::Float2}, //UV
			});
		va->SetVertexBuffer(vbo);
		unsigned int indices[3 * 1] = {
				0, 1, 2,
		};
		auto ibo = Hexy::IndexBuffer::Create(indices, 6);
		va->SetIndexBuffer(ibo);

		return va;
	}

	std::shared_ptr<VertexArray> MeshFactory::Cube()
	{

		const float vertices[(3 + 2 + 3 + 3) * 36] = {
			// position	      // uv		// normal		 // tangent	
			-0.5, -0.5, 0.5,  0.0, 1.0,  0.0, 0.0, 1.0,    1.0, 0.0, 0.0,
			0.5, -0.5, 0.5,   1.0, 1.0,  0.0, 0.0, 1.0,    1.0, 0.0, 0.0,
			-0.5, 0.5, 0.5,   0.0, 0.0,  0.0, 0.0, 1.0,    1.0, 0.0, 0.0,
			-0.5, 0.5, 0.5,   0.0, 0.0,  0.0, 0.0, 1.0,    1.0, 0.0, 0.0,
			0.5, -0.5, 0.5,   1.0, 1.0,  0.0, 0.0, 1.0,    1.0, 0.0, 0.0,
			0.5, 0.5, 0.5,    1.0, 0.0,  0.0, 0.0, 1.0,    1.0, 0.0, 0.0,
			-0.5, 0.5, 0.5,   0.0, 1.0,  0.0, 1.0, 0.0,    1.0, 0.0, 0.0,
			0.5, 0.5, 0.5,    1.0, 1.0,  0.0, 1.0, 0.0,    1.0, 0.0, 0.0,
			-0.5, 0.5, -0.5,  0.0, 0.0,  0.0, 1.0, 0.0,    1.0, 0.0, 0.0,
			-0.5, 0.5, -0.5,  0.0, 0.0,  0.0, 1.0, 0.0,    1.0, 0.0, 0.0,
			0.5, 0.5, 0.5,    1.0, 1.0,  0.0, 1.0, 0.0,    1.0, 0.0, 0.0,
			0.5, 0.5, -0.5,   1.0, 0.0,  0.0, 1.0, 0.0,    1.0, 0.0, 0.0,
			-0.5, 0.5, -0.5,  1.0, 0.0,  0.0, 0.0, -1.0,   -1.0, 0.0, 0.0,
			0.5, 0.5, -0.5,   0.0, 0.0,  0.0, 0.0, -1.0,   -1.0, 0.0, 0.0,
			-0.5, -0.5, -0.5, 1.0, 1.0,  0.0, 0.0, -1.0,   -1.0, 0.0, 0.0,
			-0.5, -0.5, -0.5, 1.0, 1.0,  0.0, 0.0, -1.0,   -1.0, 0.0, 0.0,
			0.5, 0.5, -0.5,   0.0, 0.0,  0.0, 0.0, -1.0,   -1.0, 0.0, 0.0,
			0.5, -0.5, -0.5,  0.0, 1.0,  0.0, 0.0, -1.0,   -1.0, 0.0, 0.0,
			-0.5, -0.5, -0.5, 0.0, 1.0,  0.0, -1.0, 0.0,   1.0, 0.0, 0.0,
			0.5, -0.5, -0.5,  1.0, 1.0,  0.0, -1.0, 0.0,   1.0, 0.0, 0.0,
			-0.5, -0.5, 0.5,  0.0, 0.0,  0.0, -1.0, 0.0,   1.0, 0.0, 0.0,
			-0.5, -0.5, 0.5,  0.0, 0.0,  0.0, -1.0, 0.0,   1.0, 0.0, 0.0,
			0.5, -0.5, -0.5,  1.0, 1.0,  0.0, -1.0, 0.0,   1.0, 0.0, 0.0,
			0.5, -0.5, 0.5,   1.0, 0.0,  0.0, -1.0, 0.0,   1.0, 0.0, 0.0,
			0.5, -0.5, 0.5,   0.0, 1.0,  1.0, 0.0, 0.0,    0.0, 0.0, -1.0,
			0.5, -0.5, -0.5,  1.0, 1.0,  1.0, 0.0, 0.0,    0.0, 0.0, -1.0,
			0.5, 0.5, 0.5,    0.0, 0.0,  1.0, 0.0, 0.0,    0.0, 0.0, -1.0,
			0.5, 0.5, 0.5,    0.0, 0.0,  1.0, 0.0, 0.0,    0.0, 0.0, -1.0,
			0.5, -0.5, -0.5,  1.0, 1.0,  1.0, 0.0, 0.0,    0.0, 0.0, -1.0,
			0.5, 0.5, -0.5,   1.0, 0.0,  1.0, 0.0, 0.0,    0.0, 0.0, -1.0,
			-0.5, -0.5, -0.5, 0.0, 1.0,  -1.0, 0.0, 0.0,   0.0, 0.0, 1.0,
			-0.5, -0.5, 0.5,  1.0, 1.0,  -1.0, 0.0, 0.0,   0.0, 0.0, 1.0,
			-0.5, 0.5, -0.5,  0.0, 0.0,  -1.0, 0.0, 0.0,   0.0, 0.0, 1.0,
			-0.5, 0.5, -0.5,  0.0, 0.0,  -1.0, 0.0, 0.0,   0.0, 0.0, 1.0,
			-0.5, -0.5, 0.5,  1.0, 1.0,  -1.0, 0.0, 0.0,   0.0, 0.0, 1.0,
			-0.5, 0.5, 0.5,   1.0, 0.0,  -1.0, 0.0, 0.0,   0.0, 0.0, 1.0,
		};
		auto va = Hexy::VertexArray::Create();

		auto vbo = Hexy::VertexBuffer::Create(vertices, sizeof(vertices));
		vbo->SetLayout({
			{Hexy::ElementType::Float3}, //POSITION
			{Hexy::ElementType::Float2}, //UV
			{Hexy::ElementType::Float3}, //normal
			{Hexy::ElementType::Float3}, //tangent
			});
		va->SetVertexBuffer(vbo);
		unsigned int indices[36] = {
			0,1,2,		3,4,5,
			6,7,8,		9,10,11,
			12,13,14,	15,16,17,
			18,19,20,	21,22,23,
			24,25,26,	27,28,29,
			30,31,32,	33,34,35
		};
		auto ibo = Hexy::IndexBuffer::Create(indices, 36);
		va->SetIndexBuffer(ibo);

		return va;
	}

	std::shared_ptr<VertexArray> MeshFactory::FullScreenQuad()
	{
		const float quadVertices[(3 + 2) * 4] = {
			-1,  -1, 0, 	0, 0,
			 1,   1, 0, 	1, 1,
			 1,  -1, 0, 	1, 0,
			-1,   1, 0, 	0, 1
		};
		auto quadVA = Hexy::VertexArray::Create();

		std::shared_ptr<Hexy::VertexBuffer> vbo = Hexy::VertexBuffer::Create(quadVertices, sizeof(quadVertices));
		vbo->SetLayout({
			{Hexy::ElementType::Float3}, //POSITION
			{Hexy::ElementType::Float2}, //UV
			});
		quadVA->SetVertexBuffer(vbo);
		unsigned int indices[3 * 2] = {
				2, 1, 0,
				0, 1, 3
		};
		std::shared_ptr<Hexy::IndexBuffer> ibo = Hexy::IndexBuffer::Create(indices, 6);
		quadVA->SetIndexBuffer(ibo);
		return quadVA;
	}

	std::shared_ptr<VertexArray> MeshFactory::CreatePrimitive(PrimitiveType type)
	{
		switch (type) {
		case PLANE:
			return Plane();
		case TRIANGLE:
			return Triangle();
		case CUBE:
			return Cube();
		case FULL_SCREEN_QUAD:
			return FullScreenQuad();
		}
	}
}