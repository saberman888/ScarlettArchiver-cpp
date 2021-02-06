#pragma once

#include "PushShift.hpp"
//#include "curlpp/cURLpp.hpp"
#include "ParseOptions.hpp"
#include "Misc.hpp"
#include "SubredditMetadata.hpp"
#include "Logger.hpp"

#include "Reddit/Image.hpp"
#include "Reddit/Video.hpp"
#include "Reddit/SelfPost.hpp"
#include "Reddit/Galleries.hpp"

#include <boost/archive/text_oarchive.hpp>

#include <ctime>
#include <string>
#include <map>
#include <utility>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <memory>
#include <vector>
#include <iosfwd>
#include <type_traits>
#include <omp.h>


namespace ScarlettArchiver {
	class Scarlett
	{
	public:
		Scarlett(int argc, char* argv[]);

		// Where we're going to keep our subreddit metadata
		std::unique_ptr<SubredditMetadata> sub;

		/*
			Retrieves the posts from the next 24 hours, and increments StartDate by 24 hours for another iteration.
		*/
		nlohmann::json Next();

		/*
		*	Calls directly to SubredditMetadata's HasNext
		* 
		*	@see SubredditMetadata::HasNext()
		*/
		inline bool HasNext()
		{
			return sub.get()->HasNext();
		}
	
		void Read(const nlohmann::json& source);

		void Write(const nlohmann::json& src, std::string filename);
		void Write(const std::string& buff, std::string filename);
		void WriteMetadata();

		template<class T>
		void Write(std::filesystem::path destination, std::string filename, std::shared_ptr<T> post)
		{
			static_assert(std::is_base_of<RedditAsset::RedditCommon, T>::value, "post does not derive from RedditAssett::RedditCommon");
			std::ofstream out(destination.string() + "/" + filename);
			boost::archive::text_oarchive bta(out);
			bta << *post.get();
		}

	
		std::vector< std::shared_ptr<RedditAsset::RedditCommon> > posts;	

		// Where we're going to store sub paths
		std::filesystem::path SubStorePath;
		std::shared_ptr<spdlog::logger> salog;
	};
}