#include "Scarlett/Client/RateTracker.hpp"

namespace Scarlett::Client
{
	bool RateTracker::TimeUp()
	{
		if(!StartTime)
		{
			if(StartTime.value() >= (StartTime.value() + std::chrono::hours(1)))
			{
				return true;
			}
		}
		return false;
	}
	void RateTracker::UpdateCache()
	{
		while (Delta() > minimum_time_interval)
		{
			Cache.pop_back();
		}
	}

	void RateTracker::waitifnecessary(int n)
	{
		// Get an interval if there's any
        	Millisecond interval = GetLatestInterval();

		// Multiply the number of tries by 200ms and make sure it's under 1 hour
		Millisecond waitTimes = Millisecond(200 * n);
		waitTimes = waitTimes.count() < 3600 * 1000 ? waitTimes : Millisecond(3600 * 100);

		std::this_thread::sleep_for(interval);
		std::this_thread::sleep_for(waitTimes);

	}

	RateTracker::RateTracker(int max_rate_minute_limit, std::optional<int> max_time)
	{

		this->minimum_time_interval = Millisecond(60000 / max_rate_minute_limit);
	}

	HttpResponse RateTracker::Send(const URI& srcUri, const HttpRequest& req)
	{
		web::http::client::http_client cl{ srcUri.authority().to_string() };
		HttpResponse hr;

		if(TimeUp())
		{
			scarlettOOTThrow("Error: Token's time has expired");
		}

		if(Tries >= MaxTries)
		{
			scarlettThrow("Error: Max number of requests reached");
		}

		bool complete = false;
		int tries = 0;
		while (!complete && tries < MaxTries)
		{
            		// Depending on how many retries and how long the last attempt was wait if neccessary
			waitifnecessary(tries);

            		// Next, initiate a request, time it, and push it into cache
			auto beginTime = Now();
			hr = cl.request(req).get();
			Millisecond enddiff = Now() - beginTime;
			Cache.emplace_front(enddiff);

			tries++;

			if (hr.status_code() != 429 || hr.status_code() == 200)
			{
				complete = true;
				break;
			}
		}

		return hr;
	}
};
