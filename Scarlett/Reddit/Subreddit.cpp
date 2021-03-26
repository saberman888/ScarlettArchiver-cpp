#include "Subreddit.hpp"
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
		using namespace std::chrono_literals;

		log->info("Fetching subreddit posts...");
		// Retrieve the next batch of posts by plugging StartDate into after and StartDate incremented by 24 hours into before.
		// I want to be more specific when I have SearchSubmissions call for these twenty four hours instead of plugging in EndDate into before because,
		// I think It might retrieve more data

		// Also measure time between the push shift request because, sometimes returns that have very few elements
		// can cause future requests to end in a code 429.
		auto t1 = std::chrono::system_clock::now();
		auto result = Vun::PushShift::SearchSubmissions(StringMap{
		  {"after", std::to_string(sub->DatePointer)},
		  {"before", std::to_string(sub->DatePointer += 86400)},
		  {"metadata", "true"},
		  {"size", "500"},
		  {"subreddit", sub->Subreddit}
			});
		auto t2 = std::chrono::system_clock::now();
		auto milliSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
		//std::cout << "Request time spent: " << milliSeconds.count() << std::endl;
		if (milliSeconds.count() < 670)
		{
			std::this_thread::sleep_for(550ms);
		}

		if (result.status_code() != 200)
		{
			auto msg = "Failed to fetch data from PushShift: " + u8(result.reason_phrase()) + " " + std::to_string(result.status_code());
			scarlettThrow(msg);
		}

		// Increment CurrentPointedDate by 24 hours so we can ready for the next call.
		sub->DatePointer += 86400;
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
				tempStats.Append<Gallery>();
				Add(potentialPost);
			}
			else if (Reddit::Video::IsVideo(element)) {
				log->info("Found a Video");
				auto potentialPost = std::make_shared<Video>(element);
				tempStats.Append<Video>();
			}
			else if (Reddit::SelfPost::IsSelfPost(element)) {
				log->info("Found a Self Post");
				auto potentialPost = std::make_shared<SelfPost>(element);
				tempStats.Append<SelfPost>();
				Add(potentialPost);
			}
			else {
				log->info("Found a Link");
				auto potentialPost = std::make_shared<BaseTypes::Link>(element);
				tempStats.Append<BaseTypes::Link>();
				Add(potentialPost);
			}
		}
		log->info("Updated stats");
		sub->stats.Update(tempStats);
	}
	void Subreddit::WriteAll()
	{
		using namespace std::filesystem;
		using namespace boost::serialization;

		if (!exists(SubStorePath))
			create_directories(SubStorePath);

		serializeData(*sub.get(), "metadata", (SubStorePath / "metadata.xml"));

		for (std::vector<std::shared_ptr<BaseTypes::Linkable>>::iterator it = posts.begin(); it != posts.end(); it++)
		{

			if (std::dynamic_pointer_cast<Video>(*it))
			{
				auto vid = std::dynamic_pointer_cast<Video>(*it);
				WritePost(vid, "Video");

			}
			else if (std::dynamic_pointer_cast<BaseTypes::Link>(*it)) {
				auto link = std::dynamic_pointer_cast<BaseTypes::Link>(*it);
				WritePost(link, "Link");
			}
			else if (std::dynamic_pointer_cast<Gallery>(*it))
			{
				auto gallery = std::dynamic_pointer_cast<Gallery>(*it);
				WritePost(gallery, "Gallery");
			}
			else if (std::dynamic_pointer_cast<SelfPost>(*it)) {
				auto selfpost = std::dynamic_pointer_cast<SelfPost>(*it);
				WritePost(selfpost, "SelfPost");
			}
		}
		posts.clear();
		posts.shrink_to_fit();
	}
}