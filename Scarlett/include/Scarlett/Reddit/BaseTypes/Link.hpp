#pragma once

#include <optional>
#include "Thing.hpp"
#include "../../Media/Imgur.hpp"
#include "../../Media/Content.hpp"
#include "../../Internal/Logger.hpp"
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/optional.hpp>

namespace Scarlett::Reddit
{
	class Link : public Thing, protected Logger
	{
	public:
		Link() = default;
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


		inline const String getDomain() { return Domain; }
		inline const String getTitle() { return Title; }
		inline Media::Content getURL() { return URL; }

		virtual inline const Kind getType() { return Kind::Link; }
	protected:
		String Domain;
		String Title;
		Media::Content URL;
		void Read(const JsonValue& json);
		std::optional<String> ImgurClientId{ std::nullopt };
	private:
		
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			using namespace boost::serialization;
			ar& make_nvp("Thing", base_object<Thing>(*this));
			ar& make_nvp("Domain", Domain);
			ar& make_nvp("Title", Title);
			ar& make_nvp("Hint", Hint);
		}
	};
	
};
