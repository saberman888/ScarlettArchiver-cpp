#include "Scarlett.hpp"

namespace ScarlettArchiver
{
	Scarlett::Scarlett(int argc, char* argv[]) 
	{
		InitLogs();

		Options = ScarlettOptions::ParseOptions(argc, argv);
		sub = std::make_shared<Subreddit>(Options);
	}
	void Scarlett::Run()
	{
		do {
			try {
				auto NextIteration = sub->Next();
				sub->Read(NextIteration);

#pragma omp parallel for
				for (std::vector<std::shared_ptr<RedditAsset::RedditCommon>>::iterator it = sub->posts.begin(); it != sub->posts.end(); it++)
				{
					sub->Write(sub->SubStorePath, (*it)->Id + ".txt", *it);
				}
			}
			catch (ScarlettPostException& e) {
				printException(e);
			}
			catch (PostRetrievalFailure& e) {
				printException(e);
			}
			catch (ScarlettException& e) {
				printException(e);
			}
			catch (std::exception& e) {
				printException(e);
			}

		} while (!sub->HasNext());
	}
}