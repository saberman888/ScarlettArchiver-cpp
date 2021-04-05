#include "IntervalTracker.hpp"

namespace Vun::Internal
{	
	void BasicClient::UpdateCache()
	{
		while((delta() > max_interval_time) && !Cache.empty())
		{
			Cache.pop_front();
		}
	}

	void BasicClient::waitifnecessary(int n)
	{
		Millisecond interval = 0;
		if(Cache.size() > 5)
			interval = latestInterval();
		interval = Millisecond::max(interval, 2 * n);
		interval = Millisecond::min(interval, 3600);

		if(interval > max_interval_time)
			std::this_thread::sleep_for(interval);
			
	}

	HttpResponse BasicClient::Send(const HttpRequest& req)
	{

		HttpClient cl;
		HttpResponse hr;

		bool complete = false;
		int tries = 0;
		while(!complete &&  tries < 20)
		{
			auto beginTime = std::chrono::system_clock::now();
			hr = cl.request(req).get();
			std::chrono::milliseconds  enddiff = std::chrono::system_clock::now() - beginTime();
			TotalRequests++;

			if(hr.status_code() == 200)
				break;
		}


	}
}
