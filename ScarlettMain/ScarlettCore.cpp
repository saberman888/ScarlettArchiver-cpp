#include "ScarlettCore.hpp"
#include <iostream>

namespace Scarlett
{
	ScarlettCore::ScarlettCore(int argc, char* argv[])
	{
		InitLogs();

		Options = ScarlettOptions::ParseOptions(argc, argv);
		sub = std::make_shared<Reddit::Subreddit>(Options.Subreddit, Options.StartTime, Options.EndTime);
		log = GetGlobalLogger();
	}
	void ScarlettCore::Run()
	{
		do {
			try {
				sub->Next();

				if (sub->size() >= 275|| !sub->HasNext()) {
					log->info("275 posts reached. Writing them all");
					//sub->WriteAll();
				}

			}
			catch (std::exception& e) {
				printException(e);
			}

		} while (sub->HasNext());
	}
}
