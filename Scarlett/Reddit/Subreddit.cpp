#include "Subreddit.hpp"
#include <iostream>

namespace Scarlett::Reddit {

	Subreddit::Subreddit(const std::string Subreddit, const std::string Start, const std::string End)
	{
		sub = std::make_unique<SubredditMetadata>(Subreddit, Start, End);
		log->info(sub->Subreddit + " has been instantiated.");

		SubStorePath = std::filesystem::current_path() / "subreddits" / sub->Subreddit;
		log->info("Storing at " + SubStorePath.string());
	}
	Subreddit::Subreddit(const std::filesystem::path source) : SubStorePath(source)
	{
		sub = std::make_unique<SubredditMetadata>();
		Load();
	}


	JSON::value Subreddit::Next()
	{
		using namespace std::chrono_literals;

		log->info("Fetching subreddit posts...");
		// Retrieve the next batch of posts by plugging StartDate into after and StartDate incremented by 24 hours into before.
		// I want to be more specific when I have SearchSubmissions call for these twenty four hours instead of plugging in EndDate into before because,
		// I think It might retrieve more data

		auto result = Client::PushShift::SearchSubmissions(StringMap{
		  {"after", std::to_string(sub->DatePointer)},
		  {"before", std::to_string(sub->DatePointer += 86400)},
		  {"metadata", "true"},
		  {"size", "500"},
		  {"subreddit", sub->Subreddit}
			});

		if (result.status_code() != 200)
		{
			scarlettThrow("Failed to fetch data from PushShift: " + u8(result.reason_phrase()) + " " + std::to_string(result.status_code()));
		}

		// Increment CurrentPointedDate by 24 hours so we can ready for the next call.
		sub->DatePointer += 86400;
		log->info("Incrementing by 24 hours for next fetch");

		return result.extract_json().get();
	}

	void Subreddit::Load()
	{
		// First load metadata
		if (std::filesystem::exists(SubStorePath / "metadata.xml"))
		{
			sub.reset(Internal::DeSerialize<SubredditMetadata*>(SubStorePath / "metadata.xml", "metadata"));
		}
		else {
			scarlettThrow("Couldn't find SubredditMetadata.xml in " + SubStorePath.string());
		}
	}

	void Subreddit::Read(const JSON::value& source) {
		for (auto element : source.at("data"_u).as_array())
		{
			log->info("Reading Point: " + u8(element.at("id"_u).as_string()));
			std::cout << "Reading Point: " << u8(element.at("id"_u).as_string()) << std::endl;

			log->info("Reading element...");
			if (Gallery::IsGallery(element))
			{
				log->info("Found a Gallery");
				auto potentialPost = boost::make_shared<Gallery>(element);
				Add(potentialPost);
			}
			else if (Reddit::Video::IsVideo(element)) {
				log->info("Found a Video");
				auto potentialPost = boost::make_shared<Video>(element);
				Add(potentialPost);
			}
			else if (Reddit::SelfPost::IsSelfPost(element)) {
				log->info("Found a Self Post");
				auto potentialPost = boost::make_shared<SelfPost>(element);
				Add(potentialPost);
			}
			else {
				log->info("Found a Link");
				auto potentialPost = boost::make_shared<BaseTypes::Link>(element);
				Add(potentialPost);
			}
		}
	}
	void Subreddit::WriteAll(bool clear)
	{
		using namespace std::filesystem;
		using namespace boost::serialization;

		if (!exists(SubStorePath))
			create_directories(SubStorePath);

		Internal::Serialize(SubStorePath / "metadata.xml", *sub, "metadata");

		for (std::vector<boost::shared_ptr<BaseTypes::Linkable>>::iterator it = posts.begin(); it != posts.end(); it++)
		{
			try {
				if (boost::dynamic_pointer_cast<BaseTypes::Link>(*it)) {

					if (boost::dynamic_pointer_cast<Gallery>(*it))
					{
						auto gal = boost::dynamic_pointer_cast<Gallery>(*it);
						WritePost(gal, "Gallery");
						WriteMedia(gal);
					}
					else if (boost::dynamic_pointer_cast<Video>(*it))
					{
						auto vid = boost::dynamic_pointer_cast<Video>(*it);
						WritePost(vid, "Video");
						WriteMedia(vid);
					}
					else {
						auto link = boost::dynamic_pointer_cast<BaseTypes::Link>(*it);
						WritePost(link, "Link");
						WriteMedia(link);
					}

				}
				else if (boost::dynamic_pointer_cast<SelfPost>(*it)) {
					auto selfpost = boost::dynamic_pointer_cast<SelfPost>(*it);
					WritePost(selfpost, "SelfPost");
				}
			}
			catch (ScarlettException& e) {
				printException(e);
			}
			catch (std::exception& e) {
				printException(e);
			}
		}
		if (clear) {
			posts.clear();
			posts.shrink_to_fit();
		}
	}
}
