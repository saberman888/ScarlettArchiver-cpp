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
				const std::filesystem::path galPath = mediaPath / toString(post->getId());
				std::filesystem::create_directories(galPath);
				
				std::vector<Media::Content> images = post->GetImages();

				for (auto i = 0; i < images.size()-1; i++)
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
						for(auto& data : data_vec)
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
					for(auto& data : data_vec)
					{
						out << data;
					}
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
			//scout << "Writing "_u << post->getId() << " to "_u << toScarlettString(destination.string()) << "\n"_u;
			Internal::Serialize<boost::shared_ptr<T>>(destination / toString(filename), post, toString(tag));

		}
	};
}
