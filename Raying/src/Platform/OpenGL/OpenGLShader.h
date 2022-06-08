#pragma once

#include <string>
#include <glm/glm.hpp>

#include "Raying/Renderer/Shader.h"

namespace Raying {

	class Raying_API OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

		virtual const std::string& GetName() const override { return _name; }

		void UploadUniformInt(const std::string name, int value);
		void UploadUniformIntArray(const std::string name, int* values, uint32_t count);

		void UploadUniformFloat(const std::string name, float value);
		void UploadUniformFloat2(const std::string name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string name, const glm::vec4& value);

		void UploadUniformMat3(const std::string name, const glm::mat4& matrix);
		void UploadUniformMat4(const std::string name, const glm::mat4& matrix);
	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<unsigned int, std::string> PreProcess(const std::string& source);

		void CompileOrGetVulkanBinaries(const std::unordered_map<unsigned int, std::string>& shaderSources);
		void CompileOrGetOpenGLBinaries();
		void CreateProgram();
		void Reflect(unsigned int stage, const std::vector<uint32_t>& shaderData);
	private:
		uint32_t m_RendererID;
		std::string _filePath;
		std::string _name;

		std::unordered_map<unsigned int, std::vector<uint32_t>> _vulkanSPIRV;
		std::unordered_map<unsigned int, std::vector<uint32_t>> _openGLSPIRV;

		std::unordered_map<unsigned int, std::string> _openGLSourceCode;
	};

}
