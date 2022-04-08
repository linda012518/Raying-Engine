#pragma once

#include <stdint.h>

#include "Raying/Core.h"

namespace Raying {

	class Raying_API VertexBuffer
	{
	public:
		~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);
	};

	class Raying_API IndexBuffer
	{
	public:
		~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t size);
	};

}