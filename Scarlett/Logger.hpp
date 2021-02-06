#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/fmt/bin_to_hex.h"

#include <string>

namespace ScarlettArchiver
{
	void InitLogs();

	enum SevLevel
	{
		trace = 1,
		debug,
		info,
		warn,
		err,
		crit
	};
	void write(std::shared_ptr<spdlog::logger> log, SevLevel sl, std::string msg);
};
