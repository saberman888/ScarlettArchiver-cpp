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

		void Read(const JSON::value& source);
		void WriteAll();

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