#pragma once

#include <string>
#include "RedditCommon.hpp"
#include "Images/Imgur.hpp"
#include <boost/serialization/export.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace ScarlettArchiver::RedditAsset
{
	class Link : public RedditCommon
	{
	public:
		/**
		* An empty constructor because, boost's serialization requires it.
		*/
		Link(){}
		Link(const nlohmann::json& json, SerializationMethod sm = SerializationMethod::Text);

		/**
		* Initializes a link post, but with ImgurClientId incase you want to run imgur links through the Imgur API
		*/
		Link(const nlohmann::json& json, std::string
			ImgurClientId, SerializationMethod sm = SerializationMethod::Text);

		/**
		* Returns the post's URL if it's a normal image. If it's an image from Imgur, it returns a direct image using the Imgur API
		*/
		std::string GetImage();
	private:
		std::string ImgurClientId;

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::base_object<RedditCommon>(*this);
		}
	};
	
};
