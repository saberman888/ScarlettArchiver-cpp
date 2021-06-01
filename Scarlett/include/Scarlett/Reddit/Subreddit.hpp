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
	class Subreddit : public BaseTypes::Listing<BaseTypes::Postable>, protected Logger
	{
	public:
		Subreddit(const std::string Subreddit, const std::string Start, const std::string End);

		// Where we're going to keep our subreddit metadata


		/*
			Retrieves the posts from the next 24 hours, and increments StartDate by 24 hours for another iteration.
		*/
		void Next() override;

		/*
		*	Calls directly to SubredditMetadata's HasNext
		*
		*	@see SubredditMetadata::HasNext()
		*/
		inline bool HasNext() override
		{
			return sub->HasNext();
		}

  private:
		std::vector< boost::shared_ptr<BaseTypes::Linkable> > posts;
		std::filesystem::path SubStorePath;
		std::unique_ptr<SubredditMetadata> sub;
		void Read(const JSON::value& source);
		void Load();
		void WriteAll(bool clear = true);

		template<class T>
		void Add(boost::shared_ptr<T> Post)
		{
			static_assert(
				std::is_base_of<BaseTypes::Linkable, T>::value &&
				std::is_base_of<BaseTypes::Postable, T>::value
				);

			sub->stats.Append<T>();

			if (posts.size() > 0)
			{
				for (decltype(posts)::const_iterator it = posts.begin(); it != posts.end(); it++)
				{
					// internal creation date
					auto icd = boost::dynamic_pointer_cast<BaseTypes::Postable>(*it);

					if (Post->CreatedUTC > icd->CreatedUTC)
					{
						posts.emplace(it, Post);
						return;
					}
				}
				posts.push_back(Post);
			}
			else {
				posts.push_back(Post);
			}
		}


		template<class T>
		void WriteMedia(const boost::shared_ptr<T> post)
		{
			using namespace std::filesystem;

			const auto mediaPath = SubStorePath / "media";
			create_directories(mediaPath);
			

			if constexpr (std::is_same<Gallery, T>::value)
			{
				const std::filesystem::path galPath = mediaPath / post->Id;
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
						auto filename = galPath / (post->Id + "_" + std::to_string(i) + "." + c.Extension());
						std::ofstream out(filename.string(), std::ios::binary | std::ios::out);
						out << c.GetStringContent();
					}
				}
			}
			else if constexpr (std::is_same<BaseTypes::Link, T>::value)
			{
				auto sc = post->URL.FetchContent();
				if (sc != 200)
				{
					scarlettThrow("Failed to download content from Link, " + post->URL.GetURLString());
				}

				if (post->URL.ContentType() == "image") {
					auto filename = mediaPath / path(post->Id + "." + post->URL.Extension());
					std::ofstream out(filename.string(), std::ios::binary | std::ios::out);
					out << post->URL.GetStringContent();
				}
			}
		}


		template<class T>
		void WritePost(boost::shared_ptr<T> post, const std::string tag)
		{
			using namespace std::filesystem;

			auto tempTime = *std::gmtime(&post->CreatedUTC);

			auto destination = SubStorePath /  path(formatTime(tempTime, "%Y")) / path(formatTime(tempTime, "%m")) / formatTime(tempTime, "%d");
			auto filename = post->Id + ".xml";
			std::filesystem::create_directories(destination);
			std::cout << "Writing " << post->Id << " to " << destination.string() << std::endl;
			Internal::Serialize<boost::shared_ptr<T>>(destination / filename, post, "obj");

		}

	};


  std::ifstream& operator>>(std::ifstream& in, Subreddit& sub);
  std::ofstream& operator>>(std::ofstream& out, Subrddit& sub);
}
