#pragma once
#include "Application.h"

namespace Hexy 
{
	enum ClearFlags : unsigned int 
	{
		NONE = 0,
		COLOR = 1 << 0,
		DEPTH = 1 << 1,
		STENCIL = 1 << 2
	};

	class Framebuffer
	{
	private:
		unsigned int m_buffer = 0;
		unsigned int m_colorAttachment, m_depthAttachment;
		int width, height;
		unsigned int m_samples;
		bool m_swapBufferTarget;
	public:
		Framebuffer(int width, int height, unsigned int samples = 0, bool swapBufferTarget = false);
		void Bind();
		void BindAndClear(unsigned int flags);
		void Unbind();
		void Resize(int width, int height);
		static std::shared_ptr<Framebuffer> Create(int width, int height, unsigned int samples = 0, bool swapBufferTarget = false);

		inline int GetWidth() { return width; }
		inline int GetHeight() { return height; }
		inline float GetAspect() { return (float)width / (float)height; }

		inline unsigned int GetColorAttachment() { return m_colorAttachment; }
		inline unsigned int GetDepthAttachment() { return m_colorAttachment; }
	};
}