#pragma once


#include "BaseTypes/Listing.hpp"
#include "SubredditMetadata.hpp"

namespace Scarlett::Reddit {
    class SCDLL Subreddit : public Listing<Thing>, protected Logger
	{
	public:
		Subreddit(const String Subreddit, const String Start, const String End);

		bool Next();


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
