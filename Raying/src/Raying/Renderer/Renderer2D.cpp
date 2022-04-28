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
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DSorage* _data;

	void Renderer2D::Init()
	{
		Raying_Profile_FUNCTION();

		_data = new Renderer2DSorage();

		_data->VAO = VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,  0.0f, 1.0f
		};

		Ref<VertexBuffer> vbo = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		vbo->SetLayout({
			{ShaderAttribute::Position, ShaderDataType::Float3},
			{ShaderAttribute::UV1, ShaderDataType::Float2}
			});
		_data->VAO->AddVertexBuffer(vbo);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> ibo = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		_data->VAO->SetIndexBuffer(ibo);

		_data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		_data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		_data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		_data->TextureShader->Bind();
		_data->TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		Raying_Profile_FUNCTION();

		delete _data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera & camera)
	{
		Raying_Profile_FUNCTION();

		_data->TextureShader->Bind();
		_data->TextureShader->SetMat4("_ViewProjection", camera.GetVPMatrix());
	}

	void Renderer2D::EndScene()
	{
		Raying_Profile_FUNCTION();

	}

	void Renderer2D::DrawQuad(const glm::vec2 & position, const glm::vec2 & size, const glm::vec4 & color)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3 & position, const glm::vec2 & size, const glm::vec4 & color)
	{
		Raying_Profile_FUNCTION();

		_data->TextureShader->SetFloat4("u_Color", color);
		_data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		_data->TextureShader->SetMat4("_Transform", transform);

		_data->VAO->Bind();
		RendererCommand::DrawIndexed(_data->VAO);
	}

	void Renderer2D::DrawQuad(const glm::vec2 & position, const glm::vec2 & size, const Ref<Texture2D> texture)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3 & position, const glm::vec2 & size, const Ref<Texture2D> texture)
	{
		Raying_Profile_FUNCTION();

		_data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));

		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		_data->TextureShader->SetMat4("_Transform", transform);


		_data->VAO->Bind();
		RendererCommand::DrawIndexed(_data->VAO);
	}

}