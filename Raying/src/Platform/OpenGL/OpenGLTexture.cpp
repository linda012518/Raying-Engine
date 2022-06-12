#include "hzpch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>

#include <glad/glad.h>

namespace Raying {

	OpenGLTexture::OpenGLTexture(uint32_t width, uint32_t height)
		: _width(width), _height(height)
	{
		Raying_Profile_FUNCTION();

		_internalFormat = GL_RGBA8;
		_dataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &_rendererID);
		glTextureStorage2D(_rendererID, 1, _internalFormat, _width, _height);

		glTextureParameteri(_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture::OpenGLTexture(const std::string & path)
		: _path(path)
	{
		Raying_Profile_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);//设置颠倒上下像素排列

		stbi_uc* data = nullptr;
		{
			Raying_Profile_SCOPE("stbi_load -->> OpenGLTexture");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
			Raying_Core_Assert(data, "Failed to load image!");
		}

		_width = width;
		_height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		else if (channels == 1)
		{
			internalFormat = GL_RED;
			dataFormat = GL_RED;
		}

		_internalFormat = internalFormat;
		_dataFormat = dataFormat;

		glCreateTextures(GL_TEXTURE_2D, 1, &_rendererID);
		glTextureStorage2D(_rendererID, 1, internalFormat, _width, _height);

		glTextureParameteri(_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(_rendererID, 0, 0, 0, _width, _height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture::~OpenGLTexture()
	{
		Raying_Profile_FUNCTION();

		glDeleteTextures(1, &_rendererID);
	}

	void OpenGLTexture::SetData(void * data, uint32_t size)
	{
		Raying_Profile_FUNCTION();

		uint32_t bpp = _dataFormat == GL_RGBA ? 4 : 3;
		Raying_Core_Assert(size == bpp * _width * _height, "Data must be entire texture!");
		glTextureSubImage2D(_rendererID, 0, 0, 0, _width, _height, _dataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture::Bind(uint32_t slot) const
	{
		Raying_Profile_FUNCTION();

		glBindTextureUnit(slot, _rendererID);
	}

}
