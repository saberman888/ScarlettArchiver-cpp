#pragma once

#include "../Reddit/RedditAssets.hpp"
#include <iosfwd>

namespace Scarlett
{
	class ScarlettCore
	{
	public:

		ScarlettCore(int argc, char* argv[]);
		void Run();
		struct ScarlettOptions::POptions Options;
		std::shared_ptr<Reddit::Subreddit> sub;
		std::shared_ptr<spdlog::logger> log;
	};
}
