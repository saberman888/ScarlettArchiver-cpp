#pragma once

#include <string>
#include <cstring>
#include <ctime>
#include <iosfwd>
#include "ParseOptions.hpp"
#include "Misc.hpp"

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
		SubredditMetadata() : StartDate(0), EndDate(0), DatePointer(0), Videos(0), Links(0), SelfPosts(0), Galleries(0){}

		/*
			
		*/
		SubredditMetadata(const struct ScarlettOptions::POptions ops);

		std::string Subreddit;
		time_t StartDate, EndDate, DatePointer;

		// Variables to count how many different types of posts there are
		int Videos, Links, SelfPosts, Galleries;

		/*
			Checks if there is any posts we could possibly retrieve within our set dates.
		*/
		inline bool HasNext()
		{
			return (DatePointer >= EndDate);
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

	private:
		/*
		 Scans the provided dates in string format into Unix Epoch Time, and assigns them to
		 Scarlett::StartDate and Scarlett::EndDate. It uses the strptime provided in Time.hpp
		
		 @param Dates in string format with the provided date format: YYYY:MM:DD
		*/
		void InitializeDates(std::string StartDate, std::string EndDate);

		// TODO: Expand the available date formats SM can take

	};
};
