#pragma once

#include "Raying/Core/Core.h"
#include "RendererAPI.h"

#include <memory>
#include <glm/glm.hpp>

namespace Raying {

	class Raying_API RendererCommand
	{
	public:
		inline static void Init()
		{
			_rendererAPI->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			_rendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			_rendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			_rendererAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vetrexArray, uint32_t count = 0)
		{
			_rendererAPI->DrawIndexed(vetrexArray, count);
		}

	private:
		static Scope<RendererAPI> _rendererAPI;
	};

}