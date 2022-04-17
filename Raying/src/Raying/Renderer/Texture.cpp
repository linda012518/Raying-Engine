#include "hzpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Raying {

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				Raying_Core_Assert(false, "RendererAPI::Nono is currently not support~");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return std::make_shared<OpenGLTexture>(path);
		}

		Raying_Core_Assert(false, "Unknow RendererAPI!");
		return nullptr;
	}

}