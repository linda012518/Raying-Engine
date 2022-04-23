#include "hzpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RendererCommand.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Raying {

	struct Renderer2DSorage
	{
		Ref<VertexArray> VAO;
		Ref<Shader> FlatColorShader;
	};

	static Renderer2DSorage* _data;

	void Renderer2D::Init()
	{
		_data = new Renderer2DSorage();

		_data->VAO = Raying::VertexArray::Create();

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		Raying::Ref<Raying::VertexBuffer> vbo;
		vbo.reset(Raying::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		vbo->SetLayout({
			{Raying::ShaderAttribute::Position, Raying::ShaderDataType::Float3}
			});
		_data->VAO->AddVertexBuffer(vbo);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Raying::Ref<Raying::IndexBuffer> ibo;
		ibo.reset(Raying::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		_data->VAO->SetIndexBuffer(ibo);

		_data->FlatColorShader = Raying::Shader::Create("assets/shaders/FlatColor.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete _data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera & camera)
	{
		std::dynamic_pointer_cast<OpenGLShader>(_data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(_data->FlatColorShader)->UploadUniformMat4("_ViewProjection", camera.GetVPMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(_data->FlatColorShader)->UploadUniformMat4("_Transform", glm::mat4(1.0f));
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2 & position, const glm::vec2 & size, const glm::vec4 & color)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3 & position, const glm::vec2 & size, const glm::vec4 & color)
	{
		std::dynamic_pointer_cast<OpenGLShader>(_data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(_data->FlatColorShader)->UploadUniformFloat4("u_Color", color);

		_data->VAO->Bind();
		RendererCommand::DrawIndexed(_data->VAO);
	}

}