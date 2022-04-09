#pragma once

#include "Raying/Renderer/Buffer.h"

namespace Raying {

	class Raying_API OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const BufferLayout& GetLayout() const override { return _layout; };
		virtual void SetLayout(const BufferLayout& layout) override { _layout = layout; };

	private:
		uint32_t _rendererID;
		BufferLayout _layout;
	};

	class Raying_API OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const override { return _count; }

	private:
		uint32_t _rendererID;
		uint32_t _count;
	};

}