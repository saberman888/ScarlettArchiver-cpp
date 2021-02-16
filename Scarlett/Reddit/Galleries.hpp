#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include "StringOps.hpp"
#include "Images/Imgur.hpp"
#include "Image.hpp"
#include "Comment.hpp"
#include "nlohmann/json.hpp"

#include <boost/serialization/export.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>

namespace ScarlettArchiver::RedditAsset
{
	/**
	 The Gallery class holds images from Reddit Galleries and Imgur Galleries.
	*/
	class Gallery : public RedditCommon
	{
	public:
		Gallery(){}
		// Initializes and then passes a json string into Read(const std::string& json)
		Gallery(const nlohmann::json& json);
		Gallery(const nlohmann::json& json, std::string ImgurClientId);
		std::shared_ptr<CommentListing> replies;

		/**
		 This simply returns a vector full of Image URLs if its not an Imgur URL.
		 If this is an Imgur album, it runs through the Imgur API and gets the URLs directly and returns a vector full of them.
		*/
		const std::vector<std::string> GetImages();
		static bool IsGallery(const nlohmann::json& json); 

		bool operator==(const Gallery& other);
		bool operator!=(const Gallery& other);
	private:
		/**
		 Reads Json data into Gallery. If it's a Reddit album then, it reads the provided json. If it's an Imgur album, it justs calls RedditCommon::Read only; 
		 you'd have to call GetImages to resolve the URL instead.
		*/
		void Read(const nlohmann::json& json);
		std::string ImgurClientId;

		// Where all the images will be stored
		std::vector<std::string> Images;

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::base_object<RedditCommon>(*this);
			ar& Images;
		}
	};
};

