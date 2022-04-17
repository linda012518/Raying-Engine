#pragma once

#include "Raying/Core.h"
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

		inline static void SetClearColor(const glm::vec4& color)
		{
			_rendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			_rendererAPI->Clear();
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vetrexArray)
		{
			_rendererAPI->DrawIndexed(vetrexArray);
		}

	private:
		static RendererAPI* _rendererAPI;
	};

}