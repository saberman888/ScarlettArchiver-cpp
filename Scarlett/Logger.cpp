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

	void write(std::shared_ptr<spdlog::logger> log, SevLevel sl, std::string msg)
	{
		switch (sl)
		{
		case trace:
			log->trace(msg);
			break;
		case debug:
			log->debug(msg);
			break;
		case info:
			log->info(msg);
			break;
		case warn:
			log->warn(msg);
			break;
		case err:
			log->error(msg);
			break;
		case crit:
			log->critical(msg);
			break;
		default:
			log->warn(msg);
			break;
		}
	}
}