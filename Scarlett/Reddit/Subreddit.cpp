#include "Reddit/Subreddit.hpp"
#include <iostream>

namespace ScarlettArchiver::RedditAsset {

	Subreddit::Subreddit(const struct ScarlettOptions::POptions& cmdOptions)
	{
		sub = std::make_unique<SubredditMetadata>(cmdOptions);
		log->info("Scarlett Constructor initiated");

		SubStorePath = std::filesystem::path("subreddits") / sub->Subreddit;
		log->info("Storing at " + SubStorePath.string());
	}

	JSON::value Subreddit::Next()
	{

		log->info("Fetching subreddit posts...");

		// Copy DatePointer into before and increment it by 24 hours so, we can use it in our PushShift request
		struct tm before = sub->DatePointer;
		before.tm_mday += 1;

		// Retrieve the next batch of posts by plugging StartDate into after and StartDate incremented by 24 hours into before.
		// I want to be more specific when I have SearchSubmissions call for these twenty four hours instead of plugging in EndDate into before because,
		// I think It might retrieve more data
		auto result = Vun::PushShift::SearchSubmissions(StringMap{
		  {"after", std::to_string(mktime(&sub->DatePointer))},
		  {"before", std::to_string(mktime(&before))},
		  {"metadata", "true"},
		  {"size", "500"},
		  {"subreddit", sub->Subreddit}
			});

		if (result.status_code() != 200)
		{
			scarlettThrow("Failed to fetch data from PushShift, code: " + result.status_code());
		} 

		// Increment CurrentPointedDate by 24 hours so we can ready for the next call.
		sub->DatePointer.tm_mday += 1;
		log->info("Incrementing by 24 hours for next fetch");

		return result.extract_json().get();
	}


	void Subreddit::Read(const JSON::value& source) {
		SubredditMetadata tempStats;
#pragma omp parallel for
		for (auto element : source.at("data"_u).as_array())
		{
			log->info("Reading Point: " + ToU8String(element.at("id"_u).as_string()));
			std::cout << "Reading Point: " << ToU8String(element.at("id"_u).as_string()) << std::endl;

			log->info("Reading element...");
			if (Gallery::IsGallery(element))
			{
				log->info("Found a Gallery");
				auto potentialPost = std::make_shared<Gallery>(element);
				tempStats.Galleries += 1;
				Add(potentialPost);
			}
			else if (RedditAsset::Video::IsVideo(element)) {
				log->info("Found a Video");
				auto potentialPost = std::make_shared<Video>(element);
				tempStats.Videos += 1;
#pragma omp critical (getvideoinfo)
				{
					log->info("Getting dash info");
					auto Directvideo = std::dynamic_pointer_cast<Video>(potentialPost);
					Directvideo.get()->GetVideoInfo();
				}
				Add(potentialPost);
			}
			else if (RedditAsset::SelfPost::IsSelfPost(element)) {
				log->info("Found a Self Post");
				auto potentialPost = std::make_shared<SelfPost>(element);
				tempStats.SelfPosts += 1;
				Add(potentialPost);
			}
			else {
				log->info("Found a Link");
				auto potentialPost = std::make_shared<BaseTypes::Link>(element);
				tempStats.Links += 1;
				Add(potentialPost);
			}
		}
#pragma omp critical(updateStats)
		{
			log->info("Updated stats");
			sub->UpdateStats(tempStats);
		}

	}
	void Subreddit::WriteAll()
	{
#pragma omp parallel for
		for (std::vector<std::shared_ptr<BaseTypes::Linkable>>::iterator it = posts.begin(); it != posts.end(); it++)
		{
			//Write(SubStorePath, (*it)->Id + ".txt", *it);
		}
		posts.clear();
		posts.shrink_to_fit();
	}
}