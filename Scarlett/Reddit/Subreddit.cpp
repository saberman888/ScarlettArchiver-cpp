#include "Reddit/Subreddit.hpp"
#include <iostream>

namespace Scarlett::Reddit {

	Subreddit::Subreddit(const struct ScarlettOptions::POptions& cmdOptions)
	{
		sub = std::make_unique<SubredditMetadata>(cmdOptions);
		log->info(sub->Subreddit + " has been instantiated.");

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
		struct RedditStatistics tempStats;
		for (auto element : source.at("data"_u).as_array())
		{
			log->info("Reading Point: " + u8(element.at("id"_u).as_string()));
			std::cout << "Reading Point: " << u8(element.at("id"_u).as_string()) << std::endl;

			log->info("Reading element...");
			if (Gallery::IsGallery(element))
			{
				log->info("Found a Gallery");
				auto potentialPost = std::make_shared<Gallery>(element);
				tempStats.Update<Gallery>();
				posts.push_back(potentialPost);
			}
			else if (Reddit::Video::IsVideo(element)) {
				log->info("Found a Video");
				auto potentialPost = std::make_shared<Video>(element);
				tempStats.Update<Video>();

				log->info("Getting dash info");
				auto Directvideo = std::dynamic_pointer_cast<Video>(potentialPost);
				//Directvideo.get()->Fetch();

				posts.push_back(potentialPost);
			}
			else if (Reddit::SelfPost::IsSelfPost(element)) {
				log->info("Found a Self Post");
				auto potentialPost = std::make_shared<SelfPost>(element);
				tempStats.Update<SelfPost>();
				posts.push_back(potentialPost);
			}
			else {
				log->info("Found a Link");
				auto potentialPost = std::make_shared<BaseTypes::Link>(element);
				tempStats.Update<BaseTypes::Link>();
				posts.push_back(potentialPost);
			}
		}
		log->info("Updated stats");
		// TODO: Implement stat updater
		//sub->stats(tempStats);
	}
	void Subreddit::WriteAll()
	{
		for (std::vector<std::shared_ptr<BaseTypes::Linkable>>::iterator it = posts.begin(); it != posts.end(); it++)
		{
			//Write(SubStorePath, (*it)->Id + ".txt", *it);
		}
		posts.clear();
		posts.shrink_to_fit();
	}
}