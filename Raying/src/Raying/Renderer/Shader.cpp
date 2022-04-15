#include "hzpch.h"
#include "Shader.h"
#include "Raying/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Raying {

	Shader* Shader::Create(std::string& vs, std::string& fs)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			Raying_Core_Assert(false, "RendererAPI::Nono is currently not support~");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vs, fs);
		}

		Raying_Core_Assert(false, "Unknow RendererAPI!");
		return nullptr;
	}

}