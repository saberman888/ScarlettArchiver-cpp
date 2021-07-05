#pragma once

#include "../Internal/Types.hpp"
namespace Scarlett::Client
{
	/*
	* RateTracker's duty is to keep track of requests done by classes like PushShift, and make sure they dont break rate limits.
	* 
	* TODO: Re-implement things like total requests, total time and more to handle things like refreshing tokens and etc.
	*/
    class SCDLL RateTracker
	{
	public:
		using MinuteRate = int;
		using Second = std::chrono::seconds;
		/*
		* This constructor takes the max amound of acceptable requests per minute, and it divides it by 
		* 60,000 to get the time difference between each requests in milliseconds
		*/
		RateTracker(const MinuteRate mmr);
		RateTracker(const MinuteRate mmr, const Second mht);
		RateTracker(RateTracker& rt);

		HttpResponse Send(const URI& source, const HttpRequest& hr);

		/*
		* Set the maxumum ammount of tries to retry after a request fails
		*/
		void setMaxTries(const int n);
		int MaxTries();
		void setMaxTime(const Second time);
		Second MaxTime();
		void Track();

	private:
		class _rtImpl;
		std::unique_ptr<_rtImpl> impl;
	};
};
