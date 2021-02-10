#pragma once

#include "Reddit/Subreddit.hpp"

namespace ScarlettArchiver
{
	class Scarlett
	{
	public:
		Scarlett(int argc, char* argv[]);
		void Run();
		struct ScarlettOptions::POptions Options;
		std::shared_ptr<Subreddit> sub;
	};
}
