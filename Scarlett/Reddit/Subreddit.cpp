#include "Reddit/Subreddit.hpp"
#include <iostream>

namespace ScarlettArchiver {

	Subreddit::Subreddit(int argc, char* argv[])
	{
		struct ScarlettOptions::POptions cmdOptions = ScarlettOptions::ParseOptions(argc, argv);
		sub = std::make_unique<SubredditMetadata>(cmdOptions);
		salog = GetGlobalLogger();
		salog->info("Scarlett Constructor initiated");

		SubStorePath = std::filesystem::path("subreddits") / sub.get()->Subreddit;
		salog->info("Storing at " + SubStorePath.string());
	}

	nlohmann::json Subreddit::Next()
	{

		salog->info("Fetching subreddit posts...");

		// Retrieve the next batch of posts by plugging StartDate into after and StartDate incremented by 24 hours into before.
		// I want to be more specific when I have SearchSubmissions call for these twenty four hours instead of plugging in EndDate into before because,
		// I think It might retrieve more data
		auto Returnjson = PushShift::SearchSubmissions(StringMap{
		  {"after", std::to_string(sub->DatePointer)},
		  {"before", std::to_string(sub->DatePointer + TwentyFourHours)},
		  {"metadata", "true"},
		  {"size", "500"},
		  {"subreddit", sub->Subreddit}
			});

		// Increment CurrentPointedDate by 24 hours so we can ready for the next call.
		sub->DatePointer += TwentyFourHours;
		salog->info("Incrementing by 24 hours for next fetch");

		return Returnjson;
	}


	void Subreddit::Read(const nlohmann::json& source) {
#pragma omp parallel section
		{
			SubredditMetadata tempStats;
#pragma omp parallel for
			for (nlohmann::json json : source.at("data"))
			{
				salog->info("Reading Point: " + json.at("id").get<std::string>());
				std::cout << "Reading Point: " << json.at("id").get<std::string>() << std::endl;
				std::shared_ptr<RedditAsset::RedditCommon> element;

				salog->info("Reading json...");
				if (RedditAsset::Gallery::IsGallery(json))
				{
					salog->info("Found a Gallery");
					element = std::make_shared<RedditAsset::Gallery>(json);
					tempStats.Galleries += 1;
				}
				else if (RedditAsset::Video::IsVideo(json)) {
					salog->info("Found a Video");
					element = std::make_shared<RedditAsset::Video>(json);
					tempStats.Videos += 1;
#pragma omp critical (getvideoinfo)
					{
						salog->info("Getting dash info");
						auto Directvideo = std::dynamic_pointer_cast<RedditAsset::Video>(element);
						Directvideo.get()->GetVideoInfo();
					}
				}
				else if (RedditAsset::SelfPost::IsSelfPost(json)) {
					salog->info("Found a Self Post");
					element = std::make_shared<RedditAsset::SelfPost>(json);
					tempStats.SelfPosts += 1;
				}
				else {
					salog->info("Found a Link");
					element = std::make_shared<RedditAsset::Link>(json);
					tempStats.Links += 1;
				}
#pragma omp critical (Postappending)
				{
					Write(json, "element.json");
					salog->info("Added element to Vec");
					posts.push_back(element);
				}
			}
#pragma omp critical()
			{
				salog->info("Updated stats");
				sub.get()->UpdateStats(tempStats);
			}
		}
	}
}