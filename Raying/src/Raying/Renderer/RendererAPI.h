#pragma once

#include "Raying/Core/Core.h"
#include "VertexArray.h"

#include <memory>
#include <glm/glm.hpp>

namespace Raying {

	class Raying_API RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};

	public:
		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vetrexArray) = 0;

		inline static API GetAPI() { return _api; }
		static Scope<RendererAPI> Create();

	private:
		static API _api;
	};

}