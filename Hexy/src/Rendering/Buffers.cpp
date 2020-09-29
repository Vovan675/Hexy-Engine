#include "pch.h"
#include "Buffers.h"

namespace Hexy
{
	//Vertex Buffer

	VertexBuffer::VertexBuffer(const void* vertices, size_t size)
	{
		glGenBuffers(1, &m_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_buffer);
	}

	void VertexBuffer::Bind() {
		glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	}

	void VertexBuffer::Unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(const void* vertices, size_t size)
	{
		return std::make_shared<VertexBuffer>(vertices, size);
	}

	//Index Buffer

	IndexBuffer::IndexBuffer(unsigned int* indices, size_t count)
	{
		m_count = count;
		glGenBuffers(1, &m_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_buffer);
	}

	void IndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
	}

	void IndexBuffer::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(unsigned int* indices, size_t count)
	{
		return std::make_shared<IndexBuffer>(indices, count);
	}
}