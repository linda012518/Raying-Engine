#pragma once

#include "Raying/Renderer/RendererAPI.h"

namespace Raying {

	class Raying_API OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vetrexArray) override;
	};
}