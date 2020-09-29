#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Hexy 
{
	class Log
	{
	private:
		static std::shared_ptr<spdlog::logger> s_coreLogger;
	public:
		static void Init();
		static std::shared_ptr<spdlog::logger> GetCoreLogger() { return s_coreLogger; }
	};
}

#define HX_CORE_TRACE(...) Hexy::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define HX_CORE_DEBUG(...) Hexy::Log::GetCoreLogger()->debug(__VA_ARGS__);
#define HX_CORE_INFO(...) Hexy::Log::GetCoreLogger()->info(__VA_ARGS__);
#define HX_CORE_WARN(...) Hexy::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define HX_CORE_ERROR(...) Hexy::Log::GetCoreLogger()->error(__VA_ARGS__);
#define HX_CORE_CRITICAL(...) Hexy::Log::GetCoreLogger()->critical(__VA_ARGS__);