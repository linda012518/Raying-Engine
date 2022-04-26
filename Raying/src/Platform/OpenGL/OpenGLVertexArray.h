#pragma once

#include "Raying/Renderer/VertexArray.h"

namespace Raying {

	class Raying_API OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& buffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& buffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() const { return _vertexBuffers; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const { return _indexBuffer; }

	private:
		uint32_t _rendererID;
		std::vector<Ref<VertexBuffer>> _vertexBuffers;
		Ref<IndexBuffer> _indexBuffer;
	};

}