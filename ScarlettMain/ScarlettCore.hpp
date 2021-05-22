#pragma once

#include <iosfwd>
#include "ParseOptions.hpp"
#include "Scarlett/Scarlett.hpp"
namespace Scarlett
{
	class ScarlettCore
	{
	public:

		ScarlettCore(int argc, char* argv[]);
		void Run();
		struct ScarlettOptions::POptions Options;
		std::shared_ptr<Scarlett::Reddit::Subreddit> sub;
		std::shared_ptr<spdlog::logger> log;
	};
}
