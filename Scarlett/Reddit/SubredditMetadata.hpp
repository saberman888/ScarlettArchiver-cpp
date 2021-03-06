#pragma once

#include <string>
#include <cstring>
#include <ctime>
#include <iosfwd>
#include <filesystem>
#include <optional>
#include <regex>

#include "ParseOptions.hpp"
#include "Misc.hpp"
#include "Logger.hpp"

namespace ScarlettArchiver
{
	// 24 hours in unix epoch seconds
	constexpr time_t TwentyFourHours = 86400;

	/*
		SubredditMetadata holds information regarding the date, position, statistics and functions manipulating said data.
	*/
	class SubredditMetadata
	{
	public:
		/*
			Empty constructor where everything is initialized to 0
		*/
		SubredditMetadata() : Videos(0), Links(0), SelfPosts(0), Galleries(0){}

		/*
			
		*/
		SubredditMetadata(const struct ScarlettOptions::POptions ops);

		std::string Subreddit;
		struct tm StartDate, EndDate, DatePointer;

		// Variables to count how many different types of posts there are
		int Videos, Links, SelfPosts, Galleries;

		/*
			Checks if there is any posts we could possibly retrieve within our set dates.
		*/
		inline bool HasNext()
		{
			return difftime( 
				mktime(&EndDate),
				mktime(&DatePointer)
				);
		}

		/*
			Returns a grand sum of all posts
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

		// A handle to access the global logger
		std::shared_ptr<spdlog::logger> salog;
	};
};
