#pragma once

#include <string>

#include "Raying/Core.h"

namespace Raying {

	class Raying_API Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Shader* Create(std::string& vs, std::string& fs);
	};

}