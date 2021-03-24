#pragma once

#include "PushShift.hpp"
#include "Video.hpp"
#include "SelfPost.hpp"
#include "Galleries.hpp"
#include "SubredditMetadata.hpp"
#include <map>
#include <utility>
#include <cstring>

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

		void Read(const JSON::value& source);
		void WriteAll();

		template<class T>
		void WritePost(std::shared_ptr<T> post, const std::string tag)
		{
			using namespace std::filesystem;

			auto tempTime = *std::gmtime(&post->CreatedUTC);

			auto destination = SubStorePath /  path(std::to_string(tempTime.tm_year)) / path(std::to_string(tempTime.tm_mon));
			std::filesystem::create_directories(destination);
			serializeData(*(post.get()), tag, std::filesystem::path(post->Id + ".xml"));
		}

		template<class T>
		void serializeData(const T& data, const std::string Tag, const std::filesystem::path filename)
		{
			std::ofstream out(filename.string());
			boost::archive::xml_oarchive xoa(out);
			xoa << boost::serialization::make_nvp(Tag.c_str(), data);
		}


		std::vector< std::shared_ptr<BaseTypes::Linkable> > posts;

		// Where we're going to store sub paths
		std::filesystem::path SubStorePath;
	};
}