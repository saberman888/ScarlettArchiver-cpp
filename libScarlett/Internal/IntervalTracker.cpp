#include "IntervalTracker.hpp"

namespace Scarlett
{
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
		Millisecond interval = interval = GetLatestInterval();
		
		// Multiply the number of tries by 200ms and make sure it's under 1 hour
		Millisecond waitTimes = Millisecond(200 * n);
		waitTimes = waitTimes.count() < 3600 * 1000 ? waitTimes : Millisecond(3600 * 100);

		std::this_thread::sleep_for(interval);
		std::this_thread::sleep_for(waitTimes);

	}

	RateTracker::RateTracker(int max_rate_minute_limit)
	{
		this->minimum_time_interval = Millisecond(60000 / max_rate_minute_limit);
	}

	RateTracker::HttpResponse RateTracker::Send(const URI& srcUri, const HttpRequest& req)
	{
		web::http::client::http_client cl{ srcUri.authority().to_string() };
		HttpResponse hr;

		bool complete = false;
		int tries = 0;
		while (!complete && tries < MaxTries)
		{
			waitifnecessary(tries);

			auto beginTime = Now();
			hr = cl.request(req).get();
			Millisecond enddiff = Now() - beginTime;
			Cache.emplace_front(enddiff);

			tries++;

			if (hr.status_code() != 429 || hr.status_code() == 200)
			{
				complete = true;
			}
		}

		return hr;
	}
};
