#include "Logger.hpp"
#include <iostream>

namespace ScarlettArchiver
{
	void InitLogs()
	{
		try {
			auto logger = spdlog::basic_logger_mt("Scarlett Archiver Log", "logs/main.log");
			//spdlog::enable_backtrace(25);
			spdlog::flush_every(std::chrono::seconds(3));
			spdlog::set_pattern("[%H:%M:%S %z][thread %t] %v");

		}
		catch (const spdlog::spdlog_ex& ex) {
			throw;
		}
	}

	Logger::Logger()
	{
		log = spdlog::get("Scarlett Archiver Log");
	}

}