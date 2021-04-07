#include "IntervalTracker.hpp"

namespace Vun::Internal
{	
	void RateTracker::UpdateCache()
	{
		while((delta() > max_interval_time) && !Cache.empty())
		{
			Cache.pop_back();
		}
	}

	void RateTracker::waitifnecessary(int n)
	{

		Millisecond interval = this->max_interval_time;
		if(!Cache.empty())
			interval = latestInterval();
		interval = interval.count() > 200 * static_cast<long long>(n) ? interval : Millisecond(200 * n);
		interval = interval.count() < 3600 * 1000 ? interval : Millisecond(3600 * 1000);

		if(interval < max_interval_time)
			std::this_thread::sleep_for(interval);
			
	}

	RateTracker::RateTracker(int max_rate_minute_limit, std::optional<int> TotalTime)
	{
		this->max_interval_time = Millisecond(60000 / max_rate_minute_limit);
	}

	HttpResponse RateTracker::Send(const web::uri& srcUri, const HttpRequest& req)
	{

		web::http::client::http_client cl{srcUri.authority().to_string()};
		HttpResponse hr;

		bool complete = false;
		int tries = 0;
		while(!complete &&  tries < 10)
		{
			waitifnecessary(tries);

			auto beginTime = now();
			hr = cl.request(req).get();
			Millisecond enddiff = now() - beginTime;
			Cache.push_front(enddiff);

			TotalRequests++;

			if (hr.status_code() != 200 && hr.status_code() != 429)
			{
				throw std::runtime_error(
					utility::conversions::to_utf8string(hr.reason_phrase()).c_str()
				);
			}
			else if (hr.status_code() == 200) {
				complete = true;
			}

		}

		return hr;
	}
}
