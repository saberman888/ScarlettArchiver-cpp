#pragma once

#include <optional>
#include "Linkable.hpp"
#include "Postable.hpp"
#include "Media/Imgur.hpp"
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/optional.hpp>
#include "Core/Logger.hpp"
namespace Scarlett::Reddit::BaseTypes
{
	class Link : public Linkable, public Postable, protected Logger
	{
	public:
		Link(){}
		Link(const JSON::value& json, const std::optional<std::string> ImgurClientId = std::nullopt);

		/**
		* Returns the post's URL if it's a normal image. If it's an image from Imgur, it returns a direct image using the Imgur API
		*/
		std::string GetContent();

		// PushShift's json has a hint value and I think Reddit doesn't?
		// This string is optional because I'm not totally sure if it's available on all Reddit or PushShift data
		// It's a boost optional because, a normal one can't be serialized with boost's serialization library
		boost::optional<std::string> Hint{ boost::none };

		bool operator==(Link& other);
		bool operator!=(Link& other);
	protected:
		void Read(const JSON::value& json);
		std::optional<std::string> ImgurClientId{ std::nullopt };
	private:
		
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& BOOST_SERIALIZATION_BASE_OBJECT_NVP(Linkable);
			ar& BOOST_SERIALIZATION_BASE_OBJECT_NVP(Postable);
			ar& BOOST_SERIALIZATION_NVP(Hint);
		}
	};
	
};
