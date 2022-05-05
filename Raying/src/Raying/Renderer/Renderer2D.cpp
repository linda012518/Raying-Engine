#include "hzpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RendererCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Raying {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec2 Texcoord;
		glm::vec4 Color;
		float TilingFactor;
		float TexIndex;
	};

	struct Renderer2DData
	{
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> VAO;
		Ref<VertexBuffer> VBO;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statistics Stats;
	};

	static Renderer2DData _data;

	void Renderer2D::Init()
	{
		Raying_Profile_FUNCTION();

		_data.VAO = VertexArray::Create();

		_data.VBO = VertexBuffer::Create(_data.MaxVertices * sizeof(QuadVertex));
		_data.VBO->SetLayout({
			{ShaderAttribute::Position, ShaderDataType::Float3},
			{ShaderAttribute::UV1, ShaderDataType::Float2},
			{ShaderAttribute::Color, ShaderDataType::Float4},
			{ShaderAttribute::TilingFactor, ShaderDataType::Float},
			{ShaderAttribute::TexIndex, ShaderDataType::Float}
			});
		_data.VAO->AddVertexBuffer(_data.VBO);

		_data.QuadVertexBufferBase = new QuadVertex[_data.MaxQuads];

		uint32_t* squareIndices = new uint32_t[_data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < _data.MaxIndices; i += 6)
		{
			squareIndices[i + 0] = offset + 0;
			squareIndices[i + 1] = offset + 1;
			squareIndices[i + 2] = offset + 2;

			squareIndices[i + 3] = offset + 2;
			squareIndices[i + 4] = offset + 3;
			squareIndices[i + 5] = offset + 0;

			offset += 4;
		}


		Ref<IndexBuffer> ibo = IndexBuffer::Create(squareIndices, _data.MaxIndices);
		_data.VAO->SetIndexBuffer(ibo);
		delete squareIndices;

		_data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		_data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[_data.MaxTextureSlots];
		for (uint32_t i = 0; i < _data.MaxTextureSlots; i++)
			samplers[i] = i;

		_data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		_data.TextureShader->Bind();
		_data.TextureShader->SetIntArray("u_Textures", samplers, _data.MaxTextureSlots);

		_data.TextureSlots[0] = _data.WhiteTexture;

		//这个是方块的顶点位置，批处理计算每个矩形的矩阵，乘方块顶点位置，计算出新顶点位轩
		_data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		_data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		_data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		_data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		Raying_Profile_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera & camera)
	{
		Raying_Profile_FUNCTION();

		_data.TextureShader->Bind();
		_data.TextureShader->SetMat4("_ViewProjection", camera.GetVPMatrix());

		_data.QuadIndexCount = 0;
		_data.QuadVertexBufferPtr = _data.QuadVertexBufferBase;

		_data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		Raying_Profile_FUNCTION();

		uint32_t dataSize = (uint8_t*)_data.QuadVertexBufferPtr - (uint8_t*)_data.QuadVertexBufferBase;
		_data.VBO->SetData(_data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		for (uint32_t i = 0; i < _data.TextureSlotIndex; i++)
			_data.TextureSlots[i]->Bind(i);

		RendererCommand::DrawIndexed(_data.VAO, _data.QuadIndexCount);

		_data.Stats.DrawCalls++;
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();

		_data.QuadIndexCount = 0;
		_data.QuadVertexBufferPtr = _data.QuadVertexBufferBase;

		_data.TextureSlotIndex = 1;
	}

	void Renderer2D::DrawQuad(const glm::vec2 & position, const glm::vec2 & size, const glm::vec4 & color)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3 & position, const glm::vec2 & size, const glm::vec4 & color)
	{
		Raying_Profile_FUNCTION();

		if (_data.Stats.GetTotalIndexCount() >= _data.MaxIndices)
			FlushAndReset();

		const float texIndex = 0.0f;
		const float tilingFactor = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		_data.QuadVertexBufferPtr->Position = transform * _data.QuadVertexPositions[0];
		_data.QuadVertexBufferPtr->Color = color;
		_data.QuadVertexBufferPtr->Texcoord = { 0.0f, 0.0f };
		_data.QuadVertexBufferPtr->TexIndex = texIndex;
		_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		_data.QuadVertexBufferPtr++;

		_data.QuadVertexBufferPtr->Position = transform * _data.QuadVertexPositions[1];
		_data.QuadVertexBufferPtr->Color = color;
		_data.QuadVertexBufferPtr->Texcoord = { 1.0f, 0.0f };
		_data.QuadVertexBufferPtr->TexIndex = texIndex;
		_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		_data.QuadVertexBufferPtr++;

		_data.QuadVertexBufferPtr->Position = transform * _data.QuadVertexPositions[2];
		_data.QuadVertexBufferPtr->Color = color;
		_data.QuadVertexBufferPtr->Texcoord = { 1.0f, 1.0f };
		_data.QuadVertexBufferPtr->TexIndex = texIndex;
		_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		_data.QuadVertexBufferPtr++;

		_data.QuadVertexBufferPtr->Position = transform * _data.QuadVertexPositions[3];
		_data.QuadVertexBufferPtr->Color = color;
		_data.QuadVertexBufferPtr->Texcoord = { 0.0f, 1.0f };
		_data.QuadVertexBufferPtr->TexIndex = texIndex;
		_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		_data.QuadVertexBufferPtr++;

		_data.QuadIndexCount += 6;

		_data.Stats.QuadCount++;

		//_data.TextureShader->SetFloat4("u_Color", color);
		//_data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
		//_data.WhiteTexture->Bind();

		//glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		//_data.TextureShader->SetMat4("_Transform", transform);

		//_data.VAO->Bind();
		//RendererCommand::DrawIndexed(_data.VAO);
	}

	void Renderer2D::DrawQuad(const glm::vec2 & position, const glm::vec2 & size, const Ref<Texture2D> texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3 & position, const glm::vec2 & size, const Ref<Texture2D> texture, float tilingFactor, const glm::vec4& tintColor)
	{
		Raying_Profile_FUNCTION();

		if (_data.Stats.GetTotalIndexCount() >= _data.MaxIndices)
			FlushAndReset();

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float texIndex = 0.0f;
		for (uint32_t i = 1; i < _data.TextureSlotIndex; i++)
		{
			if (*_data.TextureSlots[i].get() == *texture.get())
			{
				texIndex = (float)i;
				break;
			}
		}

		if (texIndex == 0.0f)
		{
			texIndex = (float)_data.TextureSlotIndex;
			_data.TextureSlots[_data.TextureSlotIndex] = texture;
			_data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		_data.QuadVertexBufferPtr->Position = transform * _data.QuadVertexPositions[0];
		_data.QuadVertexBufferPtr->Color = color;
		_data.QuadVertexBufferPtr->Texcoord = { 0.0f, 0.0f };
		_data.QuadVertexBufferPtr->TexIndex = texIndex;
		_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		_data.QuadVertexBufferPtr++;

		_data.QuadVertexBufferPtr->Position = transform * _data.QuadVertexPositions[1];
		_data.QuadVertexBufferPtr->Color = color;
		_data.QuadVertexBufferPtr->Texcoord = { 1.0f, 0.0f };
		_data.QuadVertexBufferPtr->TexIndex = texIndex;
		_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		_data.QuadVertexBufferPtr++;

		_data.QuadVertexBufferPtr->Position = transform * _data.QuadVertexPositions[2];
		_data.QuadVertexBufferPtr->Color = color;
		_data.QuadVertexBufferPtr->Texcoord = { 1.0f, 1.0f };
		_data.QuadVertexBufferPtr->TexIndex = texIndex;
		_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		_data.QuadVertexBufferPtr++;

		_data.QuadVertexBufferPtr->Position = transform * _data.QuadVertexPositions[3];
		_data.QuadVertexBufferPtr->Color = color;
		_data.QuadVertexBufferPtr->Texcoord = { 0.0f, 1.0f };
		_data.QuadVertexBufferPtr->TexIndex = texIndex;
		_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		_data.QuadVertexBufferPtr++;

		_data.QuadIndexCount += 6;

		_data.Stats.QuadCount++;

		//_data.TextureShader->SetFloat4("u_Color", tintColor);
		//_data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);

		//texture->Bind();

		//glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		//_data.TextureShader->SetMat4("_Transform", transform);


		//_data.VAO->Bind();
		//RendererCommand::DrawIndexed(_data.VAO);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2 & position, const glm::vec2 & size, float rotation, const glm::vec4 & color)
	{
		DrawRotatedQuad(glm::vec3(position.x, position.y, 0.0f), size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3 & position, const glm::vec2 & size, float rotation, const glm::vec4 & color)
	{
		Raying_Profile_FUNCTION();

		if (_data.Stats.GetTotalIndexCount() >= _data.MaxIndices)
			FlushAndReset();

		const float texIndex = 0.0f;
		const float tilingFactor = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		_data.QuadVertexBufferPtr->Position = transform * _data.QuadVertexPositions[0];
		_data.QuadVertexBufferPtr->Color = color;
		_data.QuadVertexBufferPtr->Texcoord = { 0.0f, 0.0f };
		_data.QuadVertexBufferPtr->TexIndex = texIndex;
		_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		_data.QuadVertexBufferPtr++;

		_data.QuadVertexBufferPtr->Position = transform * _data.QuadVertexPositions[1];
		_data.QuadVertexBufferPtr->Color = color;
		_data.QuadVertexBufferPtr->Texcoord = { 1.0f, 0.0f };
		_data.QuadVertexBufferPtr->TexIndex = texIndex;
		_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		_data.QuadVertexBufferPtr++;

		_data.QuadVertexBufferPtr->Position = transform * _data.QuadVertexPositions[2];
		_data.QuadVertexBufferPtr->Color = color;
		_data.QuadVertexBufferPtr->Texcoord = { 1.0f, 1.0f };
		_data.QuadVertexBufferPtr->TexIndex = texIndex;
		_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		_data.QuadVertexBufferPtr++;

		_data.QuadVertexBufferPtr->Position = transform * _data.QuadVertexPositions[3];
		_data.QuadVertexBufferPtr->Color = color;
		_data.QuadVertexBufferPtr->Texcoord = { 0.0f, 1.0f };
		_data.QuadVertexBufferPtr->TexIndex = texIndex;
		_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		_data.QuadVertexBufferPtr++;

		_data.QuadIndexCount += 6;

		_data.Stats.QuadCount++;

		//_data.TextureShader->SetFloat4("u_Color", color);
		//_data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
		//_data.WhiteTexture->Bind();

		//glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		//_data.TextureShader->SetMat4("_Transform", transform);

		//_data.VAO->Bind();
		//RendererCommand::DrawIndexed(_data.VAO);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2 & position, const glm::vec2 & size, float rotation, const Ref<Texture2D> texture, float tilingFactor, const glm::vec4 & tintColor)
	{
		DrawRotatedQuad(glm::vec3(position.x, position.y, 0.0f), size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3 & position, const glm::vec2 & size, float rotation, const Ref<Texture2D> texture, float tilingFactor, const glm::vec4 & tintColor)
	{
		Raying_Profile_FUNCTION();

		if (_data.Stats.GetTotalIndexCount() >= _data.MaxIndices)
			FlushAndReset();

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float texIndex = 0.0f;
		for (uint32_t i = 1; i < _data.TextureSlotIndex; i++)
		{
			if (*_data.TextureSlots[i].get() == *texture.get())
			{
				texIndex = (float)i;
				break;
			}
		}

		if (texIndex == 0.0f)
		{
			texIndex = (float)_data.TextureSlotIndex;
			_data.TextureSlots[_data.TextureSlotIndex] = texture;
			_data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		_data.QuadVertexBufferPtr->Position = transform * _data.QuadVertexPositions[0];
		_data.QuadVertexBufferPtr->Color = color;
		_data.QuadVertexBufferPtr->Texcoord = { 0.0f, 0.0f };
		_data.QuadVertexBufferPtr->TexIndex = texIndex;
		_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		_data.QuadVertexBufferPtr++;

		_data.QuadVertexBufferPtr->Position = transform * _data.QuadVertexPositions[1];
		_data.QuadVertexBufferPtr->Color = color;
		_data.QuadVertexBufferPtr->Texcoord = { 1.0f, 0.0f };
		_data.QuadVertexBufferPtr->TexIndex = texIndex;
		_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		_data.QuadVertexBufferPtr++;

		_data.QuadVertexBufferPtr->Position = transform * _data.QuadVertexPositions[2];
		_data.QuadVertexBufferPtr->Color = color;
		_data.QuadVertexBufferPtr->Texcoord = { 1.0f, 1.0f };
		_data.QuadVertexBufferPtr->TexIndex = texIndex;
		_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		_data.QuadVertexBufferPtr++;

		_data.QuadVertexBufferPtr->Position = transform * _data.QuadVertexPositions[3];
		_data.QuadVertexBufferPtr->Color = color;
		_data.QuadVertexBufferPtr->Texcoord = { 0.0f, 1.0f };
		_data.QuadVertexBufferPtr->TexIndex = texIndex;
		_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		_data.QuadVertexBufferPtr++;

		_data.QuadIndexCount += 6;

		_data.Stats.QuadCount++;

		//_data.TextureShader->SetFloat4("u_Color", tintColor);
		//_data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);

		//texture->Bind();

		//glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		//_data.TextureShader->SetMat4("_Transform", transform);


		//_data.VAO->Bind();
		//RendererCommand::DrawIndexed(_data.VAO);
	}

	void Renderer2D::ResetStats()
	{
		memset(&_data.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return _data.Stats;
	}



}