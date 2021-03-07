#pragma once

#include "PushShift.hpp"
#include "Video.hpp"
#include "SelfPost.hpp"
#include "Galleries.hpp"
#include "SubredditMetadata.hpp"
#include <map>
#include <utility>
#include <cstring>
#include <omp.h>


namespace ScarlettArchiver::RedditAsset {
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
		void Add(T post) {
#pragma omp critical
			{
				posts.push_back(post);
			}
		}
		void Read(const JSON::value& source);
		void WriteAll();

		template<class T>
		void Write(std::filesystem::path destination, std::string filename, std::shared_ptr<T> post)
		{
			static_assert((std::is_base_of<RedditAsset::Linkable, T>::value || std::is_base_of<RedditAsset::Link, T>::value), "post does not derive from RedditAsset::Linkable");

			auto tempTime = std::gmtime(post->CreatedUTC);

			destination /= std::to_string(tempTime->tm_year);
			destination /= std::to_string(tempTime->tm_mon);
			std::filesystem::create_directories(destination);

			std::ofstream out(destination.string() + "/" + filename);
			boost::archive::text_oarchive bta(out);
			bta << *post.get();
		}


		std::vector< std::shared_ptr<BaseTypes::Linkable> > posts;

		// Where we're going to store sub paths
		std::filesystem::path SubStorePath;
	};
}