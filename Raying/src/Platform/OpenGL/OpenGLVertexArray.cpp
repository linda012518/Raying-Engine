#include "hzpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Raying {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return GL_FLOAT;
			case ShaderDataType::Float2:   return GL_FLOAT;
			case ShaderDataType::Float3:   return GL_FLOAT;
			case ShaderDataType::Float4:   return GL_FLOAT;
			case ShaderDataType::Mat3:     return GL_FLOAT;
			case ShaderDataType::Mat4:     return GL_FLOAT;
			case ShaderDataType::Int:      return GL_INT;
			case ShaderDataType::Int2:     return GL_INT;
			case ShaderDataType::Int3:     return GL_INT;
			case ShaderDataType::Int4:     return GL_INT;
			case ShaderDataType::Bool:     return GL_BOOL;
		}

		Raying_Core_Assert(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		Raying_Profile_FUNCTION();

		glCreateVertexArrays(1, &_rendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		Raying_Profile_FUNCTION();

		glDeleteVertexArrays(1, &_rendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		Raying_Profile_FUNCTION();

		glBindVertexArray(_rendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		Raying_Profile_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& buffer)
	{
		Raying_Profile_FUNCTION();

		glBindVertexArray(_rendererID);
		buffer->Bind();

		const BufferLayout& layout = buffer->GetLayout();
		for (const BufferElement& element : layout)
		{
			glEnableVertexAttribArray(element.Index);
			glVertexAttribPointer(
				element.Index, element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(), (const void*)(intptr_t)element.Offset);
		}

		_vertexBuffers.push_back(buffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& buffer)
	{
		Raying_Profile_FUNCTION();

		glBindVertexArray(_rendererID);
		buffer->Bind();

		_indexBuffer = buffer;
	}

}