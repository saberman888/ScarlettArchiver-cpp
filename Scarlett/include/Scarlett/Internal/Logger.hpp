#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_sinks.h"
#include <string>

namespace Scarlett
{
	void InitLogs();

	const std::shared_ptr<spdlog::logger> GetGlobalLogger();

	struct Logger
	{
		Logger();
		std::shared_ptr<spdlog::logger> log;
	};
};
