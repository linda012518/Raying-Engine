#pragma once

#include "Base.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

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

