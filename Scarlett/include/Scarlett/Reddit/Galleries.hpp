#pragma once

#include "BaseTypes/Link.hpp"
#include "../Media/Content.hpp"
#include <boost/serialization/vector.hpp>

namespace Scarlett::Reddit
{
	/**
	 The Gallery class holds images from Reddit Galleries and Imgur Galleries.
	*/
    class SCDLL Gallery : public Link	{
	public:
		// Reads the provided json into Gallery, and if the gallery/album is an Imgur album, it retrieves the images.
		Gallery(const JsonValue& json, const std::optional<String> ImgurClientId = std::nullopt);
		/**
		 This simply returns a vector full of Image URLs.
		*/
		const std::vector<Media::Content> GetImages();

		static bool IsGallery(const JsonValue& json); 

		std::string GetContent() = delete;

	private:
		template<typename T>
		friend void Serialize(const std::filesystem::path destination, T& sourceObj, const std::string Tag);

		template<typename T>
		T DeSerialize(const std::filesystem::path source, const std::string Tag);

		/**
		 Reads Json data into Gallery. If it's a Reddit album then, it reads the provided json. If it's an Imgur album, it justs calls RedditCommon::Read only; 
		 you'd have to call GetImages to resolve the URL instead.
		*/
		void Read(const JsonValue& json);
		// Where all the images will be stored
		std::vector<Media::Content> Images;
		
		Gallery();
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version);

	};
};

