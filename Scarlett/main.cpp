
#include <iostream>
#include <fstream>
#include <queue>
#include <filesystem>
#include <omp.h>
#include "Subreddit.hpp"
#include "ParseOptions.hpp"
#include "exceptions.hpp"
int main(int argc, char* argv[])
{
	using namespace ScarlettArchiver; 
	InitLogs();
	Subreddit sc(argc, argv);
	int j = 0;
	std::filesystem::path savePath = "subreddits";
	do
	{
				
		try {
			auto i = sc.Next();
			sc.Read(i);
			// This OpenMP statement significantly sped this loop with a significant increase from around 40 minutes to 7 minutes for
			// 4 months of data
#pragma omp parallel for
			for (std::vector<std::shared_ptr<RedditAsset::RedditCommon>>::iterator it = sc.posts.begin(); it != sc.posts.end(); it++)
			{
				sc.Write(savePath / sc.sub->Subreddit, (*it)->Id + ".txt", *it);
			}

			sc.sub->PrintStats();
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
		
		if (sc.HasNext())
			break;

		j++;
	} while (!sc.HasNext());
	std::cout << "Size: " << sc.sub->GetPostTotal() << std::endl;

}
