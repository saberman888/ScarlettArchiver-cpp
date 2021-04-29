#pragma once

#include <iosfwd>
#include <optional>
#include <regex>
//#include "../Core/ParseOptions.hpp"
#include "Video.hpp"
#include "SelfPost.hpp"
#include "Galleries.hpp"


namespace Scarlett::Reddit
{

	struct RedditStatistics
	{
		int Videos{ 0 }, Links{ 0 }, SelfPosts{ 0 }, Galleries{ 0 };

		template<class T>
		void Append();
		void Update(const RedditStatistics& rs);

		inline int Sum()
		{
			return Videos + Links + SelfPosts + Galleries;
		}


		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			using namespace boost::serialization;
			ar& make_nvp("Videos", Videos);
			ar& make_nvp("Links", Links);
			ar& make_nvp("SelfPosts", SelfPosts);
			ar& make_nvp("Galleries", Galleries);
		}
	};

	template<class T>
	void RedditStatistics::Append()
	{
		if constexpr (std::is_same<Gallery, T>::value)
		{
			Galleries += 1;
		}
		else if constexpr (std::is_same<SelfPost, T>::value) {
			SelfPosts += 1;
		}
		else if constexpr (std::is_same<Video, T>::value) {
			Videos += 1;
		}
		else {
			Links += 1;
		}
	};

	/*
		SubredditMetadata holds information regarding the date, position, statistics and functions manipulating around a Subreddit
	*/
	class SubredditMetadata : protected Logger
	{
	public:

		/*
			Pass Boost program options to the constructor
		*/
		SubredditMetadata(const std::string Subreddit, const std::string StartTime, const std::string EndTime);
		SubredditMetadata() = default;

		std::string Subreddit;
		time_t StartDate { 0 }, EndDate{ 0 }, DatePointer{ 0 };

		struct RedditStatistics stats;

		/*
			Checks if there is any posts we could possibly retrieve within our set dates by, comparing the difference between the dates
		*/
		inline bool HasNext()
		{
			return !((EndDate - DatePointer) <= 0);
		}

		bool LoadFromSource(const std::filesystem::path source);


	private:

		friend boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			using namespace boost::serialization;
			ar& make_nvp("TargetSubreddit", Subreddit);
			ar& make_nvp("StartDate", StartDate);
			ar& make_nvp("EndDate", EndDate);
			ar& make_nvp("DatePointer", DatePointer);
			ar& make_nvp("Reddit-Statistics", stats);
		}

		/**
		 Scans the provided dates in string format into a Time Structure , and assigns them to
		 StartDate and EndDate. It uses the strptime provided in Misc.hpp
		
		 @param Dates in string format with the provided date formats: YYYY:MM:DD, YYYY-MM-DD, YYYY.MM.DD, YYYY/MM/DD or YYYYMMDD
		*/
		void InitializeDates(std::optional<std::string> Start = std::nullopt, std::optional<std::string> End = std::nullopt);
	};
};
