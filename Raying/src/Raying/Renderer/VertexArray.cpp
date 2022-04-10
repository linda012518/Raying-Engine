#include "hzpch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Raying {

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				Raying_Core_Assert(false, "RendererAPI::Nono is currently not support~");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return new OpenGLVertexArray();
		}

		Raying_Core_Assert(false, "Unknow RendererAPI!");
		return nullptr;
	}

}