#pragma once

#ifndef SPDLOG_FMT_EXTERNAL
  #define SPDLOG_FMT_EXTERNAL
#endif

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_sinks.h"
#include <string>
#include "Scarlett_def.hpp"

namespace Scarlett
{
    SCDLL void InitLogs();

    SCDLL const std::shared_ptr<spdlog::logger> GetGlobalLogger();

    struct SCDLL Logger
	{
		Logger();
		std::shared_ptr<spdlog::logger> log;
	};
};
