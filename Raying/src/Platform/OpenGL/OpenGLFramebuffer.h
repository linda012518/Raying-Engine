#pragma once

#include "Raying/Core/Base.h"
#include "Raying/Renderer/Framebuffer.h"

namespace Raying {

	class Raying_API OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetColorAttachmentRendererID() const override { return _colorAttachment; }
		virtual const FramebufferSpecification& GetSpecification() const override { return _specification; }

	private:
		uint32_t _rendererID;
		uint32_t _colorAttachment, _depthAttachment;
		FramebufferSpecification _specification;
	};

}