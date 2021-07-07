#pragma once

#include <iosfwd>
#include <optional>
#include <boost/serialization/access.hpp>
#include "../Internal/Types.hpp"
#include "../Internal/Logger.hpp"

namespace Scarlett::Reddit
{

	/*
		SubredditMetadata holds information regarding the date, position, statistics and functions manipulating around a Subreddit
	*/
    class SCDLL SubredditMetadata : protected Logger
	{
	public:
		using Timestamp = time_t;
		/*
			Pass Boost program options to the constructor
		*/
		SubredditMetadata(const String Subreddit, const String StartTime, const String EndTime);
		SubredditMetadata();

		void setSubreddit(const String& name);
		const String Subreddit();
		
		void setStart(const Timestamp date);
		const Timestamp Start();

		void setEnd(const Timestamp date);
		const Timestamp End();

		void setPosition(const Timestamp date);
		const Timestamp Position();
	

	private:
		String _Subreddit;
		time_t StartDate{ 0 }, EndDate{ 0 }, DatePointer{ 0 };
		friend boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version);

		/**
		 Scans the provided dates in string format into a Time Structure , and assigns them to
		 StartDate and EndDate. It uses the strptime provided in Misc.hpp
		
		 @param Dates in string format with the provided date formats: YYYY:MM:DD, YYYY-MM-DD, YYYY.MM.DD, YYYY/MM/DD or YYYYMMDD
		*/
		void InitializeDates(std::optional<String> Start = std::nullopt, std::optional<String> End = std::nullopt);
	};
};
