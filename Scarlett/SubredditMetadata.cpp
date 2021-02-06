#include "SubredditMetadata.hpp"
#include <iostream>

namespace ScarlettArchiver
{ 
	
	SubredditMetadata::SubredditMetadata(const struct ScarlettOptions::POptions ops) : StartDate(0), EndDate(0), DatePointer(0), Videos(0), Links(0), SelfPosts(0), Galleries(0)
	{
		InitializeDates(ops.StartTime, ops.EndTime);
		Subreddit = ops.Subreddit;
	}

	int SubredditMetadata::GetPostTotal()
	{
		return Galleries + Videos + SelfPosts + Links;
	}

	void SubredditMetadata::PrintStats()
	{
		std::cout << "Scanned date: " << asctime(std::gmtime(&DatePointer)) << std::endl;

		std::cout << "Statistics" << "\n";
		std::cout << "Links: " << Links << "\n";
		std::cout << "Videos: " << Videos << "\n";
		std::cout << "Galleries: " << Galleries << "\n";
		std::cout << "Self Posts: " << SelfPosts << "\n";
		std::cout << std::flush;
	}

	void SubredditMetadata::UpdateStats(const SubredditMetadata& src)
	{
		Links += src.Links;
		SelfPosts += src.SelfPosts;
		Videos += src.Videos;
		Galleries += src.Galleries;
	}

	void SubredditMetadata::InitializeDates(std::string StartDate, std::string EndDate)
	{
		// Declare two tm structs for StartDate and EndDate
		struct tm tmStart, tmEnd;
		
		// Then zero them out and convert whatever date is in there
		// to the tm struct
		std::memset(&tmStart, 0, sizeof(struct tm));
		std::memset(&tmEnd, 0, sizeof(struct tm));

		ScarlettArchiver::strptime(StartDate.c_str(), "%Y:%m:%d", &tmStart);
		ScarlettArchiver::strptime(EndDate.c_str(), "%Y:%m:%d", &tmEnd);

		// After the conversion to tm, convert the structs to unix epoch seconds.
		// Then assign them to SubredditMetadata's StartDate and EndDate
		// Assign DatePointer to StartDate since that's probably we're going to start
		this->StartDate = mktime(&tmStart) - TwentyFourHours;
		this->EndDate = mktime(&tmEnd) + TwentyFourHours;
		this->DatePointer = this->StartDate;
	}
}