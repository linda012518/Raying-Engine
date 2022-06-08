#pragma once

#include <string>
#include <optional>

namespace Raying {

	class FileDialogs
	{
	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);

	};

}
