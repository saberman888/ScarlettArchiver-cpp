#include "Scarlett/Internal/Logger.hpp"
#include <iostream>

namespace Scarlett
{
	void InitLogs()
	{
		try {
			auto ScarlettMainLogger = spdlog::basic_logger_mt("Scarlett Archiver Log", "logs/main.log");
			spdlog::register_logger(ScarlettMainLogger);
			//spdlog::enable_backtrace(25);
			spdlog::flush_every(std::chrono::seconds(3));
			ScarlettMainLogger->set_pattern("[%H:%M:%S %z][thread %t] %v");

		}
		catch (const spdlog::spdlog_ex&) {
			throw;
		}
	}

	const std::shared_ptr<spdlog::logger> GetGlobalLogger()
	{
		return spdlog::get("Scarlett Archiver Log");
	}

	Logger::Logger()
	{
		log = spdlog::get("Scarlett Archiver Log");
	}

}