#pragma once

#include <boost/serialization/optional.hpp>
#include <optional>
#include "Thing.hpp"
#include "../../Media/Content.hpp"
#include "../../Internal/Logger.hpp"
#include "../../Internal/Serializable.hpp"


namespace Scarlett::Reddit
{
    class SCDLL Link : public Thing, protected Logger
	{
	public:
		Link();
		Link(const JsonValue& json, const std::optional<String> ImgurClientId = std::nullopt);

		/**
		* Returns the post's URL if it's a normal image. If it's an image from Imgur, it returns a direct image using the Imgur API
		*/
		String GetContent();

		// PushShift's json has a hint value and I think Reddit doesn't?
		// This string is optional because I'm not totally sure if it's available on all Reddit or PushShift data
		// It's a boost optional because, a normal one can't be serialized with boost's serialization library
		boost::optional<String> Hint{ boost::none };

		bool operator==(Link& other);
		bool operator!=(Link& other);


		const String getDomain();
		const String getTitle();
		const Media::Content getURL();

	protected:
		String Domain;
		String Title;
		Media::Content URL;
		void Read(const JsonValue& json);
		std::optional<String> ImgurClientId{ std::nullopt };
	private:

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version);

	};
	
};
