#include "pch.h"
#include "VertexArray.h"
#include "DebugInfo.h"

namespace Hexy 
{
	VertexArray::VertexArray(RenderType primitiveType)
	{
		m_type = primitiveType;
		glGenVertexArrays(1, &m_buffer);
		glBindVertexArray(m_buffer);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_buffer);
	}

	void VertexArray::Bind()
	{
		glBindVertexArray(m_buffer);
	}

	void VertexArray::Unbind()
	{
		glBindVertexArray(0);
	}

	void VertexArray::Draw()
	{
		DebugInfo::DrawCalls++;
		glBindVertexArray(m_buffer);
		glDrawElements(m_type, m_indexBuffer->GetCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void VertexArray::SetVertexBuffer(std::shared_ptr<VertexBuffer> buffer)
	{
		m_vertexBuffer = buffer;
		glBindVertexArray(m_buffer);
		buffer->Bind();
		buffer->GetLayout().Use();
	}

	void VertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer> buffer)
	{
		m_indexBuffer = buffer;
		glBindVertexArray(m_buffer);
		buffer->Bind();
	}

	std::shared_ptr<VertexArray> VertexArray::Create(RenderType primitiveType)
	{
		return std::make_shared<VertexArray>(primitiveType);
	}
}