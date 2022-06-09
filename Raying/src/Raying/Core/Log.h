#pragma once

#include <glm/gtx/string_cast.hpp>

#include "Base.h"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Raying {

	class Raying_API Log
	{
	public:

		static void Init();
		//shared_ptr 允许多个指针指向同一个对象
		inline static Ref<spdlog::logger>& GetCoreLogger() { return _coreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return _clientLogger; }

	private:
		static Ref<spdlog::logger> _coreLogger;
		static Ref<spdlog::logger> _clientLogger;

	};

}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator << (OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}
template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator << (OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}
template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator << (OStream& os, const glm::qua<T, Q>& quaternio)
{
	return os << glm::to_string(quaternio);
}

#define Raying_Core_Critical(...)	::Raying::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define Raying_Core_Error(...)		::Raying::Log::GetCoreLogger()->error(__VA_ARGS__)
#define Raying_Core_Warn(...)		::Raying::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define Raying_Core_Info(...)		::Raying::Log::GetCoreLogger()->info(__VA_ARGS__)
#define Raying_Core_Trace(...)		::Raying::Log::GetCoreLogger()->trace(__VA_ARGS__)

#define Raying_Critical(...)		::Raying::Log::GetClientLogger()->critical(__VA_ARGS__)
#define Raying_Error(...)			::Raying::Log::GetClientLogger()->error(__VA_ARGS__)
#define Raying_Warn(...)			::Raying::Log::GetClientLogger()->warn(__VA_ARGS__)
#define Raying_Info(...)			::Raying::Log::GetClientLogger()->info(__VA_ARGS__)
#define Raying_Trace(...)			::Raying::Log::GetClientLogger()->trace(__VA_ARGS__)

