#include "Scarlett/Reddit/Subreddit.hpp"
#include <iostream>

namespace Scarlett::Reddit {

	Subreddit::Subreddit(const std::string Subreddit, const std::string Start, const std::string End)
	{
		sub = std::make_unique<SubredditMetadata>(Subreddit, Start, End);
		log->info("{} has been instantiated.", sub->Subreddit);
	}

	void Subreddit::Next()
	{
		using namespace std::chrono_literals;

		log->info("Fetching posts from {} between {} and {}.", sub->Subreddit, sub->DatePointer, sub->DatePointer + 86400);

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
			log->error("Error: Failed to retrieve {} posts between {} and {}", sub->Subreddit, sub->DatePointer, sub->DatePointer + 86400);
			scarlettHTTPThrow(result);
		}
		else {
			log->info("Retrieval successful.");
		}

		// Increment CurrentPointedDate by 24 hours so we can ready for the next call.
		sub->DatePointer += 86400;
		log->info("Incrementing by 24 hours for the next fetch.");
		
		Read(result.extract_json().get());

	}

	void Subreddit::Read(const JSON::value& source) {

		log->info("Reading data from fetch...");
		for (auto element : source.at("data"_u).as_array())
		{
			if (Gallery::IsGallery(element))
			{
				auto potentialPost = boost::make_shared<Gallery>(element);
				Add(potentialPost);
			}
			else if (Reddit::Video::IsVideo(element)) {
				auto potentialPost = boost::make_shared<Video>(element);
				Add(potentialPost);
			}
			else if (Reddit::SelfPost::IsSelfPost(element)) {
				auto potentialPost = boost::make_shared<SelfPost>(element);
				Add(potentialPost);
			}
			else {
				auto potentialPost = boost::make_shared<BaseTypes::Link>(element);
				Add(potentialPost);
			}
		}
	}

	void Subreddit::Save(const std::filesystem::path location, bool clear)
	{
		using namespace std::filesystem;
		using namespace boost::serialization;

		log->info("Saving posts from {}.", sub->Subreddit);
		create_directories(location);
	
		Internal::Serialize(location / "metadata.xml", *sub, "metadata");
		log->info("Serialized metadata.");

		log->info("Writing media...");
		for (std::vector<boost::shared_ptr<BaseTypes::Linkable>>::iterator it = posts.begin(); it != posts.end(); it++)
		{
			try {
				if (boost::dynamic_pointer_cast<BaseTypes::Link>(*it)) {

					if (boost::dynamic_pointer_cast<Gallery>(*it))
					{
						auto gal = boost::dynamic_pointer_cast<Gallery>(*it);
						WritePost(gal, "Gallery", location);
						WriteMedia(gal, location);
					}
					else if (boost::dynamic_pointer_cast<Video>(*it))
					{
						auto vid = boost::dynamic_pointer_cast<Video>(*it);
						WritePost(vid, "Video", location);
						WriteMedia(vid, location);
					}
					else {
						auto link = boost::dynamic_pointer_cast<BaseTypes::Link>(*it);
						WritePost(link, "Link", location);
						WriteMedia(link, location);
					}

				}
				else if (boost::dynamic_pointer_cast<SelfPost>(*it)) {
					auto selfpost = boost::dynamic_pointer_cast<SelfPost>(*it);
					WritePost(selfpost, "SelfPost", location);
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
			log->info("Deleted all previous posts");
			posts.clear();
			posts.shrink_to_fit();
		}
	}

	void Subreddit::Load(const std::filesystem::path location)
	{
		log->info("Loading existing data from {}", location.string());
		if (std::filesystem::exists(location / "metadata.xml"))
		{
			sub.reset(Internal::DeSerialize<SubredditMetadata*>(location / "metadata.xml", "metadata"));
			log->info("Metadata loaded");
		}
		else {
			scarlettThrow("Couldn't find SubredditMetadata.xml in " + location.string());
		}
	}

}
