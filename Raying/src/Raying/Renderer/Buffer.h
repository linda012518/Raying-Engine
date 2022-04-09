#pragma once

#include <stdint.h>

#include "Raying/Core.h"

namespace Raying {

	enum class Raying_API ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Mat3, Mat4, Bool
	};

	enum class Raying_API ShaderAttribute
	{
		Position = 0, Color, Normal, UV1, UV2, UV3, UV4, UV5, UV6
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return 4;
			case ShaderDataType::Float2:   return 4 * 2;
			case ShaderDataType::Float3:   return 4 * 3;
			case ShaderDataType::Float4:   return 4 * 4;
			case ShaderDataType::Mat3:     return 4 * 3 * 3;
			case ShaderDataType::Mat4:     return 4 * 4 * 4;
			case ShaderDataType::Int:      return 4;
			case ShaderDataType::Int2:     return 4 * 2;
			case ShaderDataType::Int3:     return 4 * 3;
			case ShaderDataType::Int4:     return 4 * 4;
			case ShaderDataType::Bool:     return 1;
		}

		Raying_Core_Assert(false, "Unknown ShaderDataType!");
		return 0;
	}

	static uint32_t ShaderAttriIndex(ShaderAttribute attr, uint32_t* offset)
	{
		switch (attr)
		{
			case Raying::ShaderAttribute::Position: *offset = 0  * 4; return 0;
			case Raying::ShaderAttribute::Color:	*offset = 3  * 4; return 1;
			case Raying::ShaderAttribute::Normal:	*offset = 7  * 4; return 2;
			case Raying::ShaderAttribute::UV1:		*offset = 10 * 4; return 3;
			case Raying::ShaderAttribute::UV2:		*offset = 12 * 4; return 4;
			case Raying::ShaderAttribute::UV3:		*offset = 14 * 4; return 5;
			case Raying::ShaderAttribute::UV4:		*offset = 16 * 4; return 6;
			case Raying::ShaderAttribute::UV5:		*offset = 18 * 4; return 7;
			case Raying::ShaderAttribute::UV6:		*offset = 20 * 4; return 8;
		}

		Raying_Core_Assert(false, "Unknown ShaderAttribute!");
		return -1;
	}

	struct Raying_API BufferElement
	{
		ShaderAttribute Attribute;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		uint32_t Index;
		bool Normalized;

		BufferElement() {}

		BufferElement(ShaderAttribute attri, ShaderDataType type, bool normalized = false)
			: Attribute(attri), Type(type), Size(ShaderDataTypeSize(type)), Normalized(normalized)
		{
			Index = ShaderAttriIndex(attri, &Offset);
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case Raying::ShaderDataType::Float:		return 1;
				case Raying::ShaderDataType::Float2:	return 2;
				case Raying::ShaderDataType::Float3:	return 3;
				case Raying::ShaderDataType::Float4:	return 4;
				case Raying::ShaderDataType::Int:		return 1;
				case Raying::ShaderDataType::Int2:		return 2;
				case Raying::ShaderDataType::Int3:		return 3;
				case Raying::ShaderDataType::Int4:		return 4;
				case Raying::ShaderDataType::Mat3:		return 3 * 3;
				case Raying::ShaderDataType::Mat4:		return 4 * 4;
				case Raying::ShaderDataType::Bool:		return 1;
			}

			Raying_Core_Assert(false, "Unknow ShaderDataType!");
			return 0;
		}
	};

	class Raying_API BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: _elements(elements)
		{
			CalculateStride();
		}

		inline uint32_t GetStride() const { return _stride; }
		inline const std::vector<BufferElement>& GetElements() const { return _elements; }

		std::vector<BufferElement>::const_iterator begin() const { return _elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return _elements.end(); }

	private:
		void CalculateStride()
		{
			_stride = 0;
			for (auto& element : _elements)
			{
				_stride += element.Size;
			}
		}

	private:
		std::vector<BufferElement> _elements;
		uint32_t _stride;
	};

	class Raying_API VertexBuffer
	{
	public:
		~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

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