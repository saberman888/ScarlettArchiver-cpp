#pragma once

#define SPDLOG_FMT_EXTERNAL 1

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_sinks.h"
#include <string>

namespace Scarlett
{
	void InitLogs();

	inline std::shared_ptr<spdlog::logger> GetGlobalLogger()
	{
		return spdlog::get("Scarlett Archiver Log");
	}

	struct Logger
	{
		Logger();
		std::shared_ptr<spdlog::logger> log;
	};
};
