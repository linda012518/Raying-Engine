#pragma once

#include "Raying/Renderer/Texture.h"

namespace Raying {

	class Raying_API OpenGLTexture : public Texture2D
	{
	public:
		OpenGLTexture(const std::string& path);
		virtual ~OpenGLTexture();

		virtual uint32_t GetWidth() const override { return _width; }
		virtual uint32_t GetHeight() const override { return _height; }

		virtual void Bind(uint32_t slot = 0) const override;

	private:
		std::string _path;
		uint32_t _width, _height;
		uint32_t _rendererID;
	};

}