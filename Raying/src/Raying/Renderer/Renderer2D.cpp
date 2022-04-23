#include "hzpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RendererCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Raying {

	struct Renderer2DSorage
	{
		Ref<VertexArray> VAO;
		Ref<Shader> FlatColorShader;
		Ref<Shader> TextureShader;
	};

	static Renderer2DSorage* _data;

	void Renderer2D::Init()
	{
		_data = new Renderer2DSorage();

		_data->VAO = Raying::VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,  0.0f, 1.0f
		};

		Raying::Ref<Raying::VertexBuffer> vbo;
		vbo.reset(Raying::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		vbo->SetLayout({
			{Raying::ShaderAttribute::Position, Raying::ShaderDataType::Float3},
			{Raying::ShaderAttribute::UV1, Raying::ShaderDataType::Float2}
			});
		_data->VAO->AddVertexBuffer(vbo);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Raying::Ref<Raying::IndexBuffer> ibo;
		ibo.reset(Raying::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		_data->VAO->SetIndexBuffer(ibo);

		_data->FlatColorShader = Raying::Shader::Create("assets/shaders/FlatColor.glsl");
		_data->TextureShader = Raying::Shader::Create("assets/shaders/Texture.glsl");
		_data->TextureShader->Bind();
		_data->TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		delete _data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera & camera)
	{
		_data->FlatColorShader->Bind();
		_data->FlatColorShader->SetMat4("_ViewProjection", camera.GetVPMatrix());

		_data->TextureShader->Bind();
		_data->TextureShader->SetMat4("_ViewProjection", camera.GetVPMatrix());
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
		_data->FlatColorShader->Bind();
		_data->FlatColorShader->SetFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		_data->FlatColorShader->SetMat4("_Transform", transform);

		_data->VAO->Bind();
		RendererCommand::DrawIndexed(_data->VAO);
	}

	void Renderer2D::DrawQuad(const glm::vec2 & position, const glm::vec2 & size, const Ref<Texture2D> texture)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3 & position, const glm::vec2 & size, const Ref<Texture2D> texture)
	{
		_data->TextureShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		_data->TextureShader->SetMat4("_Transform", transform);

		texture->Bind();

		_data->VAO->Bind();
		RendererCommand::DrawIndexed(_data->VAO);
	}

}