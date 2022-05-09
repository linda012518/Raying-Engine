#include "hzpch.h"
#include "FrameBuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Raying {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				Raying_Core_Assert(false, "RendererAPI::Nono is currently not support~");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLFramebuffer>(spec);
		}

		Raying_Core_Assert(false, "Unknow RendererAPI!");
		return nullptr;
	}


}