#include "Scarlett/Reddit/Subreddit.hpp"
#include "Scarlett/Client/PushShift.hpp"
#include "Scarlett/Reddit/Video.hpp"
#include "Scarlett/Reddit/Galleries.hpp"
#include "Scarlett/Media/Content.hpp"
#include "Scarlett/Reddit/BaseTypes/TextPost.hpp"
#include "Scarlett/Internal/Serializable.hpp"
#include "Scarlett/Internal/Exceptions.hpp"
#include <iostream>
#include <map>
#include <utility>
#include <cstring>
#include <chrono>
#include <thread>
#include <algorithm>
#include <fstream>

namespace Scarlett::Reddit {

	Subreddit::Subreddit(const String Subreddit, const String Start, const String End) :
		Listing()
	{
		sub = std::make_unique<SubredditMetadata>(Subreddit, Start, End);
		log->info("{} has been instantiated.", toString(sub->Subreddit()));
	}

	bool Subreddit::Next()
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
		return true;
	}

	void Subreddit::Save(const std::filesystem::path location, bool clear)
	{
		using namespace std::filesystem;
		using namespace boost::serialization;

		log->info("Saving posts from {}.", toString(sub->Subreddit()));
		create_directories(location);
	
		Serializable<SubredditMetadata>::Serialize(location / "metadata.xml", *sub, "metadata");
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
				else if (boost::dynamic_pointer_cast<TextPost>(*it)) {
					auto selfpost = boost::dynamic_pointer_cast<TextPost>(*it);
					WritePost(selfpost, "TextPost"_u, location);
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
			sub.reset(Serializable<SubredditMetadata*>::DeSerialize(location / "metadata.xml", "metadata"));
			log->info("Metadata loaded");
		}
		else {
			scarlettThrow("Couldn't find SubredditMetadata.xml in " + location.string());
		}
	}

	template<class T>
	void Subreddit::WriteMedia(const boost::shared_ptr<T> post, const std::filesystem::path location)
	{
		using namespace std::filesystem;

		const auto mediaPath = location / "media";
		create_directories(mediaPath);


		if constexpr (std::is_same<Gallery, T>::value)
		{
			const std::filesystem::path galPath = mediaPath / toString(post->getId());
			std::filesystem::create_directories(galPath);

			std::vector<Media::Content> images = post->GetImages();

			for (auto i = 0; i < images.size() - 1; i++)
			{
				Media::Content& c = images[i];
				if (c.FetchContent() != 200)
				{
					scarlettThrow("Failed to download Gallery image: " + toString(c.GetURLString()));
				}
				else {
					auto filename = galPath / (toString(post->getId()) + "_" + std::to_string(i) + "." + toString(c.Extension()));
					std::ofstream out(filename.string(), std::ios::binary | std::ios::out);

					auto data_vec = c.GetContent();
					for (auto& data : data_vec)
					{
						out << data;
					}
				}
			}
		}
		else if constexpr (std::is_same<Link, T>::value)
		{
			auto sc = post->getURL().FetchContent();
			if (sc != 200)
			{
				scarlettThrow("Failed to download content from Link, " + toString(post->getURL().GetURLString()));
			}

			if (post->getURL().ContentType() == "image"_u) {
				auto filename = mediaPath / path(toString(post->getId() + "."_u + post->getURL().Extension()));
				std::ofstream out(filename.string(), std::ios::binary | std::ios::out);

				auto data_vec = post->getURL().GetContent();
				for (auto& data : data_vec)
				{
					out << data;
				}
			}
		}
	}

	template<class T>
	void Subreddit::WritePost(boost::shared_ptr<T> post, const String tag, const std::filesystem::path location)
	{
		using namespace std::filesystem;

		auto time = post->getCreatedUTCTime();
		auto tempTime = *std::gmtime(&time);
		auto destination = location / path(formatTime(tempTime, "%Y")) / path(formatTime(tempTime, "%m")) / formatTime(tempTime, "%d");
		String filename = post->getId() + ".xml"_u;

		std::filesystem::create_directories(destination);
		//scout << "Writing "_u << post->getId() << " to "_u << toScarlettString(destination.string()) << "\n"_u;
		Internal::Serialize<boost::shared_ptr<T>>(destination / toString(filename), post, toString(tag));
	}

	template void Subreddit::WriteMedia(const boost::shared_ptr<Link> post, const std::filesystem::path location);
	template void Subreddit::WriteMedia(const boost::shared_ptr<Gallery> post, const std::filesystem::path location);

	template void Subreddit::WritePost(boost::shared_ptr<Link> post, const String tag, const std::filesystem::path location);
	template void Subreddit::WritePost(boost::shared_ptr<Gallery> post, const String tag, const std::filesystem::path location);
	template void Subreddit::WritePost(boost::shared_ptr<Comment> post, const String tag, const std::filesystem::path location);
	template void Subreddit::WritePost(boost::shared_ptr<TextPost> post, const String tag, const std::filesystem::path location);
	template void Subreddit::WritePost(boost::shared_ptr<Video> post, const String tag, const std::filesystem::path location);
}
