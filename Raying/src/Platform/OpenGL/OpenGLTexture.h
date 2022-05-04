#pragma once

#include "Raying/Renderer/Texture.h"

namespace Raying {

	class Raying_API OpenGLTexture : public Texture2D
	{
	public:
		OpenGLTexture(uint32_t width, uint32_t height);
		OpenGLTexture(const std::string& path);
		virtual ~OpenGLTexture();

		virtual uint32_t GetWidth() const override { return _width; }
		virtual uint32_t GetHeight() const override { return _height; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator == (const Texture& other) const override
		{
			return _rendererID == ((OpenGLTexture&)other)._rendererID;
		}

	private:
		std::string _path;
		uint32_t _width, _height;
		uint32_t _rendererID;

		unsigned int _internalFormat, _dataFormat;
	};

}