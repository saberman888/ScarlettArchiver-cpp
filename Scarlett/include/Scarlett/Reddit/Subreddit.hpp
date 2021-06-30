#pragma once

#include "../Client/PushShift.hpp"
#include "Video.hpp"
#include "SelfPost.hpp"
#include "Galleries.hpp"
#include "SubredditMetadata.hpp"
#include "../Media/Content.hpp"
#include "../Internal/Serializable.hpp"
#include "BaseTypes/Listing.hpp"
#include <map>
#include <utility>
#include <cstring>
#include <chrono>
#include <thread>
#include <algorithm>
#include <fstream>

namespace Scarlett::Reddit {
    class SCDLL Subreddit : public Listing<Thing>, protected Logger
	{
	public:
		Subreddit(const String Subreddit, const String Start, const String End);

		/*
			Retrieves the posts from the next 24 hours, and increments StartDate by 24 hours for another iteration.
		*/
		void Next() override;

		/*
		*	HasNext checks if there are any potential posts we could fetch.
		*	It does this by getting the differrence of the end date and current position in time
		*/
		inline bool HasNext() override
		{
			return !(
				(sub->End() - sub->Position()) <= 0
				);
		}

		void Save(const std::filesystem::path location, bool clear = true);
		void Load(const std::filesystem::path location);

  private:
		std::unique_ptr<SubredditMetadata> sub;

		template<class T>
		void WriteMedia(const boost::shared_ptr<T> post, const std::filesystem::path location)
		{
			using namespace std::filesystem;

			const auto mediaPath = location / "media";
			create_directories(mediaPath);
			

			if constexpr (std::is_same<Gallery, T>::value)
			{
				const std::filesystem::path galPath = mediaPath / post->getId();
				std::filesystem::create_directories(galPath);
				
				std::vector<Media::Content> images = post->GetImages();

				for (auto i = 0; i < images.size()-1; i++)
				{
					Media::Content& c = images[i];
					if (c.FetchContent() != 200)
					{
						scarlettThrow("Failed to download Gallery image: " + c.GetURLString());
					}
					else {
						auto filename = galPath / (post->getId() + "_" + std::to_string(i) + "." + c.Extension());
						std::ofstream out(filename.string(), std::ios::binary | std::ios::out);
						out << c.GetStringContent();
					}
				}
			}
			else if constexpr (std::is_same<Link, T>::value)
			{
				auto sc = post->getURL().FetchContent();
				if (sc != 200)
				{
					scarlettThrow("Failed to download content from Link, " + post->getURL().GetURLString());
				}

				if (post->getURL().ContentType() == "image") {
					auto filename = mediaPath / path(post->getId() + "." + post->getURL().Extension());
					std::ofstream out(filename.string(), std::ios::binary | std::ios::out);
					out << post->getURL().GetStringContent();
				}
			}
		}


		template<class T>
		void WritePost(boost::shared_ptr<T> post, const String tag, const std::filesystem::path location)
		{
			using namespace std::filesystem;

			auto time = post->getCreatedUTCTime();
			auto tempTime = *std::gmtime(&time);

			auto destination = location /  path(formatTime(tempTime, "%Y")) / path(formatTime(tempTime, "%m")) / formatTime(tempTime, "%d");
			String filename = post->getId() + ".xml"_u;
			std::filesystem::create_directories(destination);
			ucout << "Writing " << post->getId() << " to " << destination.string() << std::endl;
			Internal::Serialize<boost::shared_ptr<T>>(destination / filename, post, "obj");

		}
	};
}
