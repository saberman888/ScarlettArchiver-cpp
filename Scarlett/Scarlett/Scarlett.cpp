#include "Scarlett.hpp"
#include <iostream>

namespace ScarlettArchiver
{
	ScarlettCore::ScarlettCore(int argc, char* argv[])
	{
		InitLogs();

		Options = ScarlettOptions::ParseOptions(argc, argv);
		sub = std::make_shared<RedditAsset::Subreddit>(Options);
		log = GetGlobalLogger();
	}
	void ScarlettCore::Run()
	{
		do {
			try {
				auto NextIteration = sub->Next();
				sub->Read(NextIteration);

				if (sub->posts.size() >= 300 || !sub->HasNext()) {
					log->info("300 posts reached. Writing them all");
					sub->WriteAll();
				}


			}
			catch (std::exception& e) {
				printException(e);
			}

		} while (sub->HasNext());
	}
}