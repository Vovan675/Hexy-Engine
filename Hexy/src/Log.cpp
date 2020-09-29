#include "pch.h"
#include "Log.h"

namespace Hexy 
{
	std::shared_ptr<spdlog::logger> Log::s_coreLogger;

	void Log::Init()
    {
        std::vector<spdlog::sink_ptr> logSinks;
        logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Hexy.log", true));

        s_coreLogger = std::make_shared<spdlog::logger>("HEXY", logSinks.begin(), logSinks.end());
        spdlog::register_logger(s_coreLogger);
        s_coreLogger->set_level(spdlog::level::trace);
        s_coreLogger->flush_on(spdlog::level::trace);
        s_coreLogger->set_pattern("[%T] [%l] %n: %v");
	}
}