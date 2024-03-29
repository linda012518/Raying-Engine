#include "hzpch.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Raying {

	RendererAPI::API RendererAPI::_api = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (_api)
		{
			case RendererAPI::API::None:
				Raying_Core_Assert(false, "RendererAPI::Nono is currently not support~");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateScope<OpenGLRendererAPI>();
		}

		Raying_Core_Assert(false, "Unknow RendererAPI!");
		return nullptr;
	}

}