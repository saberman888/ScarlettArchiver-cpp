#pragma once

#include <string>
#include <optional>
#include "Linkable.hpp"
#include "Postable.hpp"
#include "Images/Imgur.hpp"
#include <boost/serialization/export.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace ScarlettArchiver::RedditAsset
{
	class Link : public Linkable, public Postable, protected Logger
	{
	public:
		/**
		* An empty constructor because, boost's serialization requires it.
		*/
		Link(){}
		Link(const JSON::value& json, const std::optional<std::string> ImgurClientId = std::nullopt);

		/**
		* Returns the post's URL if it's a normal image. If it's an image from Imgur, it returns a direct image using the Imgur API
		*/
		String GetContent();

		String Hint;

		bool operator==(Link& other);
		bool operator!=(Link& other);
	protected:
		void Read(const JSON::value& json);
		std::optional<std::string> ImgurClientId;
	private:
		
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::base_object<Linkable>(*this);
			ar& boost::serialization::base_object<Postable>(*this);
		}
	};
	
};
