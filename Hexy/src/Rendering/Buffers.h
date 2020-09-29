#pragma once
#include "Application.h"

namespace Hexy
{
	enum ElementType
	{
		Int,
		Int2,
		Int3,
		Int4,
		Float,
		Float2,
		Float3,
		Float4
	};

	class BufferLayoutElement 
	{
	public:
		ElementType type;
		int offset;

		BufferLayoutElement(ElementType type) : type(type), offset(0)
		{

		}

		int GetCount() 
		{
			switch (type) {
			case ElementType::Int:
				return 1;
			case ElementType::Int2:
				return 2;
			case ElementType::Int3:
				return 3;
			case ElementType::Int4:
				return 4;
			case ElementType::Float:
				return 1;
			case ElementType::Float2:
				return 2;
			case ElementType::Float3:
				return 3;
			case ElementType::Float4:
				return 4;
			}
		}

		int GetSize() 
		{
			switch (type) 
			{
			case ElementType::Int:
				return sizeof(int);
			case ElementType::Int2:
				return sizeof(int) * 2;
			case ElementType::Int3:
				return sizeof(int) * 3;
			case ElementType::Int4:
				return sizeof(int) * 4;
			case ElementType::Float:
				return sizeof(float);
			case ElementType::Float2:
				return sizeof(float) * 2;
			case ElementType::Float3:
				return sizeof(float) * 3;
			case ElementType::Float4:
				return sizeof(float) * 4;
			}
		}
	};

	class BufferLayout 
	{
	private:
		int m_stride;
		std::vector<BufferLayoutElement> m_elements;
	public:
		BufferLayout()  {}

		BufferLayout(std::initializer_list<BufferLayoutElement> elements) : m_elements(elements) 
		{
			int offset = 0;
			for (auto element = m_elements.begin(); element < m_elements.end(); ++element)
			{
				element->offset = offset;
				offset += element->GetSize();
			}
			m_stride = offset;
		}

		int getStride() { return m_stride; }

		void Use() 
		{
			int attrib = 0;

			for (auto element = m_elements.begin(); element < m_elements.end(); ++element) 
			{
				glVertexAttribPointer(attrib, element->GetCount(), GL_FLOAT, GL_FALSE, m_stride, (const void*)element->offset);
				glEnableVertexAttribArray(attrib);
				attrib++;
			}
		}
	};

	//Vertex Buffer

	class VertexBuffer 
	{
	private:
		unsigned int m_buffer;
		BufferLayout m_layout;
	public:
		VertexBuffer(const void* vertices, size_t size);
		~VertexBuffer();

		void Bind();
		void Unbind();
		inline void SetLayout(const BufferLayout& layout) { m_layout = layout; }
		inline BufferLayout& GetLayout() { return m_layout; }

		inline unsigned int GetBuffer() { return m_buffer; }

		static std::shared_ptr<VertexBuffer> Create(const void* vertices, size_t size);
	};

	//Index Buffer

	class IndexBuffer 
	{
	private:
		unsigned int m_buffer;
		size_t m_count;
	public:
		IndexBuffer(unsigned int* indices, size_t count);
		~IndexBuffer();

		void Bind();
		void Unbind();

		inline unsigned int GetBuffer() { return m_buffer; }
		inline size_t GetCount() { return m_count; }

		static std::shared_ptr<IndexBuffer> Create(unsigned int* indices, size_t count);
	};
}