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

		virtual void Resize(float width, float height) override;
		virtual int ReadPixed(uint32_t attachmentIndex, int x, int y) override;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { Raying_Core_Assert(index < _colorAttachments.size()); return _colorAttachments[index]; }
		virtual const FramebufferSpecification& GetSpecification() const override { return _specification; }

	private:
		uint32_t _rendererID = 0;
		FramebufferSpecification _specification;

		std::vector<FramebufferTextureSpecification> _colorAttachmentSpecifications;
		FramebufferTextureSpecification _depthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> _colorAttachments;
		uint32_t _depthAttachment = 0;
	};

}
