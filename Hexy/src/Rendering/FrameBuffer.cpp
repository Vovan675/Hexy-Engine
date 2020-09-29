#include "pch.h"
#include "FrameBuffer.h"

namespace Hexy 
{
	Framebuffer::Framebuffer(int width, int height, unsigned int samples, bool swapBufferTarget) : m_samples(samples), m_swapBufferTarget(swapBufferTarget)
	{
		Resize(width, height);
	}

	void Framebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_swapBufferTarget ? 0 : m_buffer);
		glViewport(0, 0, width, height);
	}

	void Framebuffer::BindAndClear(unsigned int flags)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_swapBufferTarget ? 0 : m_buffer);
		glViewport(0, 0, width, height);

		unsigned int clearFlags = 0;
		clearFlags |= flags & ClearFlags::COLOR ? GL_COLOR_BUFFER_BIT : 0;
		clearFlags |= flags & ClearFlags::DEPTH ? GL_DEPTH_BUFFER_BIT : 0;
		clearFlags |= flags & ClearFlags::STENCIL ? GL_STENCIL_BUFFER_BIT : 0;

		glClearColor(1, 0, 1, 1);
		glClear(clearFlags);
	}

	void Framebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Resize(int width, int height)
	{
		//TODO STRUCT TO HOLD ALL PROPERTIES OF FRAMEBUFFER

		if (this->width == width && this->height == height) {
			return;
		}
		this->width = width;
		this->height = height;

		if (m_swapBufferTarget)
		{
			return;
		}

		if (m_buffer) {
			glDeleteFramebuffers(1, &m_buffer);
			glDeleteTextures(1, &m_colorAttachment);
			glDeleteTextures(1, &m_depthAttachment);
		}

		if (m_samples == 0) {
			glGenFramebuffers(1, &m_buffer);
			glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);

			glGenTextures(1, &m_colorAttachment);
			glBindTexture(GL_TEXTURE_2D, m_colorAttachment);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);

			glGenTextures(1, &m_depthAttachment);
			glBindTexture(GL_TEXTURE_2D, m_depthAttachment);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else {
			glGenFramebuffers(1, &m_buffer);
			glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);

			glGenTextures(1, &m_colorAttachment);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_colorAttachment);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, GL_RGBA16F, width, height, true);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

			glGenTextures(1, &m_depthAttachment);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_depthAttachment);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, GL_DEPTH24_STENCIL8, width, height, true);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		}
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_colorAttachment, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, m_depthAttachment, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "FRAMEBUFFER ERROR!!!";
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	std::shared_ptr<Framebuffer> Framebuffer::Create(int width, int height, unsigned int samples, bool swapBufferTarget)
	{
		return std::make_shared<Framebuffer>(width, height, samples, swapBufferTarget);
	}
}
