#pragma once

#include "PushShift.hpp"
#include "Video.hpp"
#include "SelfPost.hpp"
#include "Galleries.hpp"
#include "SubredditMetadata.hpp"
#include <map>
#include <utility>
#include <cstring>
#include <chrono>
#include <thread>

namespace Scarlett::Reddit {
	class Subreddit : protected Logger
	{
	public:
		Subreddit(const struct ScarlettOptions::POptions& cmdOptions);

		// Where we're going to keep our subreddit metadata
		std::unique_ptr<SubredditMetadata> sub;

		/*
			Retrieves the posts from the next 24 hours, and increments StartDate by 24 hours for another iteration.
		*/
		JSON::value Next();

		/*
		*	Calls directly to SubredditMetadata's HasNext
		*
		*	@see SubredditMetadata::HasNext()
		*/
		inline bool HasNext()
		{
			return sub->HasNext();
		}

		template<class T>
		void Add(std::shared_ptr<T> Post)
		{
			static_assert(
				std::is_base_of<BaseTypes::Linkable, T>::value &&
				std::is_base_of<BaseTypes::Postable, T>::value
				);

			if (posts.size() > 0)
			{
				for (decltype(posts)::iterator it = posts.begin(); it != posts.end(); it++)
				{
					// internal creation date
					auto icd = std::dynamic_pointer_cast<BaseTypes::Postable>(*it);

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

		void Read(const JSON::value& source);
		void WriteAll(bool clear = true);

		template<class T>
		void WriteMedia(const std::shared_ptr<T> post)
		{
			using namespace std::filesystem;

			/*static_assert(
				std::is_base_of<T, BaseTypes::Link>::value || std::is_same<T, BaseTypes::Link>::value
				);*/

			if (!exists(SubStorePath / "media"))
				create_directories(SubStorePath / "media");
			const auto mediaPath = SubStorePath / "media";

			if constexpr (std::is_same<Gallery, T>::value)
			{
				if (!exists(mediaPath / post->Id))
					create_directories(mediaPath / post->Id);
				const auto galPath = mediaPath / post->Id;
				auto images = post->GetImages();
				for (std::vector<std::string>::const_iterator it = images.begin(); it != images.end(); it++)
				{
					auto downloadResult = Download(*it);
					auto file = downloadResult.extract_utf8string(true).get();

					std::ofstream out(galPath.string());
					out << file;
				}
			}
			else if constexpr (std::is_same<BaseTypes::Link, T>::value)
			{
				auto dlImage = Download(post->URL).extract_utf8string();

				std::ofstream out(mediaPath / path(post->Id + ".png"));
				out << dlImage.get();
			}
		}

		template<class T>
		void WritePost(std::shared_ptr<T> post, const std::string tag)
		{
			using namespace std::filesystem;

			auto tempTime = *std::gmtime(&post->CreatedUTC);

			auto destination = SubStorePath /  path(formatTime(tempTime, "%Y")) / path(formatTime(tempTime, "%m")) / formatTime(tempTime, "%d");
			std::filesystem::create_directories(destination);
			std::cout << "Writing " << post->Id << " to " << destination.string() << std::endl;
			serializeData(*(post.get()), tag, (destination / std::filesystem::path(post->Id + ".xml")));
		}

		template<class T>
		void serializeData(const T& data, const std::string Tag, const std::filesystem::path filename)
		{
			std::ofstream out;
			out.exceptions(std::ofstream::failbit | std::ofstream::badbit);
			try {
				out.open(filename);
				boost::archive::xml_oarchive xoa(out);
				xoa << boost::serialization::make_nvp(Tag.c_str(), data);
			}
			catch (std::system_error& e) {
				scarlettNestedThrow(e.what());
			}
			catch (std::exception& e) {
				scarlettNestedThrow(e.what());
			}
		}


		std::vector< std::shared_ptr<BaseTypes::Linkable> > posts;

		// Where we're going to store sub paths
		std::filesystem::path SubStorePath;
	};
}