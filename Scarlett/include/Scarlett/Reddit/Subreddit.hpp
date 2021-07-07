#pragma once


#include "BaseTypes/Listing.hpp"
#include "SubredditMetadata.hpp"

namespace Scarlett::Reddit {
    class SCDLL Subreddit : public Listing<Thing>, protected Logger
	{
	public:
		Subreddit(const String Subreddit, const String Start, const String End);

		/*
			Retrieves the posts from the next 24 hours, and increments StartDate by 24 hours for another iteration.
		*/
		void Next() override;

		/*
		*	HasNext checks if there are any potential posts we could fetch.
		*	It does this by getting the differrence of the end date and current position in time
		*/
		bool HasNext() override;

		void Save(const std::filesystem::path location, bool clear = true);
		void Load(const std::filesystem::path location);

  private:
		std::unique_ptr<SubredditMetadata> sub;

		template<class T>
		void WriteMedia(const boost::shared_ptr<T> post, const std::filesystem::path location);

		template<class T>
		void WritePost(boost::shared_ptr<T> post, const String tag, const std::filesystem::path location);
	};
}
