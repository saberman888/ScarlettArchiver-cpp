#include "Scarlett.hpp"
#include <iostream>

namespace Scarlett
{
	ScarlettCore::ScarlettCore(int argc, char* argv[])
	{
		InitLogs();

		Options = ScarlettOptions::ParseOptions(argc, argv);
		sub = std::make_shared<Reddit::Subreddit>(Options);
		log = GetGlobalLogger();
	}
	void ScarlettCore::Run()
	{
		do {
			try {
				auto NextIteration = sub->Next();
				sub->Read(NextIteration);

				if (sub->posts.size() >= 275|| !sub->HasNext()) {
					log->info("275 posts reached. Writing them all");
					sub->WriteAll();
				}

			}
			catch (std::exception& e) {
				printException(e);
			}

		} while (sub->HasNext());
	}
}