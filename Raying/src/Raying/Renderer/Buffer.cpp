#include "hzpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Raying {

	VertexBuffer * VertexBuffer::Create(float * vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:
				Raying_Core_Assert(false, "RendererAPI::Nono is currently not support~");
				return nullptr;
			case RendererAPI::OpenGL:
				return new OpenGLVertexBuffer(vertices, size);
		}
		Raying_Core_Assert(false, "Unknow RendererAPI!");
	}

	IndexBuffer * IndexBuffer::Create(uint32_t * indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:
				Raying_Core_Assert(false, "RendererAPI::Nono is currently not support~");
				return nullptr;
			case RendererAPI::OpenGL:
				return new OpenGLIndexBuffer(indices, size);
		}
		Raying_Core_Assert(false, "Unknow RendererAPI!");
	}

}