#include "Scarlett/Reddit/Subreddit.hpp"
#include <iostream>

namespace Scarlett::Reddit {

	Subreddit::Subreddit(const std::string Subreddit, const std::string Start, const std::string End)
	{
		sub = std::make_unique<SubredditMetadata>(Subreddit, Start, End);
		log->info("{} has been instantiated.", sub->Subreddit);

		SubStorePath = std::filesystem::current_path() / "subreddits" / sub->Subreddit;
		auto k = SubStorePath.string();
		log->info("Storing at {}", SubStorePath.string());
	}

	void Subreddit::Next()
	{
		using namespace std::chrono_literals;

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
			scarlettHTTPThrow(result);
		}

		// Increment CurrentPointedDate by 24 hours so we can ready for the next call.
		sub->DatePointer += 86400;
		
    Read(result.extract_json.get());

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
