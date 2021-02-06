#pragma once

#include <boost/program_options/config.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <string>
namespace ScarlettOptions
{
	struct POptions
	{
		std::string Subreddit;
		std::string StartTime;
		std::string EndTime;
	};

	struct POptions ParseOptions(int argc, char* argv[]);
}
