#pragma once
#include "Application.h"
#include "Buffers.h"

namespace Hexy
{
	enum RenderType : unsigned int
	{
		TRIANGLES = GL_TRIANGLES,
		LINES = GL_LINES,
		POINTS = GL_POINTS
	};

	class VertexArray 
	{
	private:
		unsigned int m_buffer;
		std::shared_ptr<VertexBuffer> m_vertexBuffer;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
		RenderType m_type;
	public:
		VertexArray(RenderType primitiveType = TRIANGLES);
		~VertexArray();
		void Bind();
		void Unbind();
		void Draw();

		void SetVertexBuffer(std::shared_ptr<VertexBuffer> buffer);
		void SetIndexBuffer(std::shared_ptr<IndexBuffer> buffer);

		inline std::shared_ptr<VertexBuffer> GetVertexBuffer() 
		{
			return m_vertexBuffer;
		}
		inline std::shared_ptr<IndexBuffer> GetIndexBuffer() 
		{
			return m_indexBuffer;
		}

		inline unsigned int GetBuffer() { return m_buffer; }

		static std::shared_ptr<VertexArray> Create(RenderType primitiveType = TRIANGLES);
	};
}