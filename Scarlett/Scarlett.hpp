#pragma once

#include "Reddit/Subreddit.hpp"
#include <iosfwd>

namespace ScarlettArchiver
{
	class Scarlett
	{
	public:

		Scarlett(int argc, char* argv[]);
		void Run();
		struct ScarlettOptions::POptions Options;
		std::shared_ptr<Subreddit> sub;
		std::shared_ptr<spdlog::logger> log;
	};
}
