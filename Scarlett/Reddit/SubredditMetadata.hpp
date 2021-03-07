#pragma once

#include <iosfwd>
#include <optional>
#include <regex>
#include "Scarlett/Misc.hpp"
#include "Scarlett/ParseOptions.hpp"
#include "Scarlett/Logger.hpp"

namespace ScarlettArchiver::RedditAsset
{
	// 24 hours in unix epoch seconds
	constexpr time_t TwentyFourHours = 86400;

	/*
		SubredditMetadata holds information regarding the date, position, statistics and functions manipulating said data.
	*/
	class SubredditMetadata : public Logger
	{
	public:
		/*
			Initializes everything to zero
		*/
		SubredditMetadata() : Videos(0), Links(0), SelfPosts(0), Galleries(0){}

		/*
			Pass Boost program options to the constructor
		*/
		SubredditMetadata(const struct ScarlettOptions::POptions ops);

		std::string Subreddit;
		struct tm StartDate, EndDate, DatePointer;

		// Variables to count how many different types of posts there are
		int Videos, Links, SelfPosts, Galleries;

		/*
			Checks if there is any posts we could possibly retrieve within our set dates by, comparing the difference between the dates
		*/
		inline bool HasNext()
		{
			return difftime( 
				mktime(&EndDate),
				mktime(&DatePointer)
				);
		}

		/*
			Returns a grand total of all posts
		*/
		int GetPostTotal();

		/*
			Print the date and how many there are of each posts.
		*/
		void PrintStats();

		/*
			Update our stats with an empty instance of SubredditMetadata

			@param const reference to SubredditMetadata
		*/
		void UpdateStats(const SubredditMetadata& src);
		
		/**
		* Writes the data stored in the class such as, Start date, End date and the statistics in metadata.json
		* 
		* @param destination - Destination path for metadata.json
		*/
		void WriteMetadata(const std::filesystem::path destination);


	private:
		/**
		 Scans the provided dates in string format into a Time Structure , and assigns them to
		 Scarlett::StartDate and Scarlett::EndDate. It uses the strptime provided in Time.hpp
		
		 @param Dates in string format with the provided date formats: YYYY:MM:DD, YYYY-MM-DD, YYYY.MM.DD, YYYY/MM/DD or YYYYMMDD
		*/
		void InitializeDates(std::optional<std::string> Start = std::nullopt, std::optional<std::string> End = std::nullopt);
	};
};
