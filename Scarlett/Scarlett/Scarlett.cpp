#include "Scarlett.hpp"
#include <iostream>

namespace ScarlettArchiver
{
	Scarlett::Scarlett(int argc, char* argv[])
	{
		InitLogs();

		Options = ScarlettOptions::ParseOptions(argc, argv);
		sub = std::make_shared<Subreddit>(Options);
		log = GetGlobalLogger();
	}
	void Scarlett::Run()
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