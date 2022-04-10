#pragma once

#include "Raying/Core.h"
#include "RendererCommand.h"

namespace Raying {

	class Raying_API Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vetrexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};

}