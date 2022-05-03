#pragma once

#include "Raying/Renderer/RendererAPI.h"

namespace Raying {

	class Raying_API OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vetrexArray, uint32_t count = 0) override;
	};
}