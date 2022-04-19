#include "hzpch.h"
#include "Shader.h"
#include "Raying/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Raying {

	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				Raying_Core_Assert(false, "RendererAPI::Nono is currently not support~");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return std::make_shared<OpenGLShader>(filepath);
		}

		Raying_Core_Assert(false, "Unknow RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vs, const std::string& fs)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				Raying_Core_Assert(false, "RendererAPI::Nono is currently not support~");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return std::make_shared<OpenGLShader>(name, vs, fs);
		}

		Raying_Core_Assert(false, "Unknow RendererAPI!");
		return nullptr;
	}

	void ShaderLibrary::Add(const std::string & name, const Ref<Shader>& shader)
	{
		Raying_Core_Assert(!Exists(name), "Shader already exists!");
		_shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		const std::string& name = shader->GetName();
		_shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string & filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string & name, const std::string & filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string & name)
	{
		Raying_Core_Assert(Exists(name), "Shader not found!");
		return _shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string & name) const
	{
		return _shaders.find(name) != _shaders.end();
	}

}