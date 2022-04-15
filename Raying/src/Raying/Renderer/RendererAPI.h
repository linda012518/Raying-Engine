#pragma once

#include "Raying/Core.h"
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
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vetrexArray) = 0;

		inline static API GetAPI() { return _api; }

	private:
		static API _api;
	};

}