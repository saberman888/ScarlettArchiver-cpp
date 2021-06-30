#include "Scarlett/Reddit/Subreddit.hpp"
#include <iostream>

namespace Scarlett::Reddit {

	Subreddit::Subreddit(const String Subreddit, const String Start, const String End)
	{
		sub = std::make_unique<SubredditMetadata>(Subreddit, Start, End);
		log->info("{} has been instantiated.", toString(sub->Subreddit()));
	}

	void Subreddit::Next()
	{
		try {
			log->info("Fetching posts from {} between {} and {}.", toString(sub->Subreddit()), sub->Position(), sub->Position() + 86400);
			auto currentposition = sub->Position();
			this->NextItems(StringMap{
			  {"after", std::to_string(currentposition)},
			  {"before", std::to_string(currentposition += 86400)},
			  {"metadata", "true"},
			  {"size", "500"},
			  {"subreddit", toString(sub->Subreddit())}
				});

			// Increment CurrentPointedDate by 24 hours so we can ready for the next call.
			sub->setPosition(currentposition += 86400);
			log->info("Incrementing by 24 hours for the next fetch.");

			log->error("Error: Failed to retrieve {} posts between {} and {}", toString(sub->Subreddit()), sub->Position(), sub->Position() + 86400);
		}
		catch (ScarlettHTTPException& e) {
			scarlettNestedThrow(e.what());
		}
	}

	void Subreddit::Save(const std::filesystem::path location, bool clear)
	{
		using namespace std::filesystem;
		using namespace boost::serialization;

		log->info("Saving posts from {}.", toString(sub->Subreddit()));
		create_directories(location);
	
		Internal::Serialize(location / "metadata.xml", *sub, "metadata");
		log->info("Serialized metadata.");

		log->info("Writing media...");
		auto posts = Posts();
		for (auto it = posts.cbegin(); it != posts.cend(); it++)
		{
			try {
				if (boost::dynamic_pointer_cast<Link>(*it)) {

					if (boost::dynamic_pointer_cast<Gallery>(*it))
					{
						auto gal = boost::dynamic_pointer_cast<Gallery>(*it);
						WritePost(gal, "Gallery"_u, location);
						WriteMedia(gal, location);
					}
					else if (boost::dynamic_pointer_cast<Video>(*it))
					{
						auto vid = boost::dynamic_pointer_cast<Video>(*it);
						WritePost(vid, "Video"_u, location);
						WriteMedia(vid, location);
					}
					else {
						auto link = boost::dynamic_pointer_cast<Link>(*it);
						WritePost(link, "Link"_u, location);
						WriteMedia(link, location);
					}

				}
				else if (boost::dynamic_pointer_cast<SelfPost>(*it)) {
					auto selfpost = boost::dynamic_pointer_cast<SelfPost>(*it);
					WritePost(selfpost, "SelfPost"_u, location);
				}
			}
			catch (ScarlettException& e) {
				printException(e);
			}
			catch (std::exception& e) {
				printException(e);
			}
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
