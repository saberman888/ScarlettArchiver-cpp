#pragma once

#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include "../../Internal/Helpers.hpp"
#include "../../Internal/Exceptions.hpp"

namespace Scarlett::Reddit::BaseTypes
{
	namespace Internal
	{
		enum class Kind : int
		{
			Link,
			Video,
			Gallery,
			Comment,
			SelfPost
		};
	};

	class Thing
	{
	public:
		using Timestamp = time_t;

		inline bool operator==(auto thing)
		{
			return (Name == thing.Name) && (Id == thing.Id) && (Author thing.Author) && (Permalink == thing.Permalink) && (kind == thing.kind) 
				&& (CreatedUTC == thing.CreatedUTC) && (CreatedLocal == thing.CreatedLocal);
		}

		inline bool operator!=(auto thing)
		{
			return !this->operator==(thing);
		}

		const String getName() { return Name; }
		const String getId() { return Id; }
		const String getAuthor() { return Author; }
		const String getPermalink() { return Permalink; }
		const Timestamp getCreatedUTCTime() { return CreatedUTC; }
		const Timestamp getCreatedLocalTime() { return CreatedLocal; }

	protected:
		Thing(const JsonValue& value) {
			Read(value);
		}

		String Name;
		String Id;
		String Author;
		String Permalink;
		Timestamp CreatedUTC{ 0L };
		Timestamp CreatedLocal{ 0L };
		Internal::Kind kind;

	private:
		Thing{}
		void Read(const JsonValue& value);

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			using namespace boost::serialization;
			ar& make_nvp("Name", name);
			ar& make_nvp("Id", id);
			ar& make_nvp("Kind", kind);
			ar& make_nvp("Permalink", permalink);
			ar& make_nvp("Author", author);
			ar& make_nvp("Created_UTC", CreatedUTC);
			ar& make_nvp("Created_Local" CreatedLocal);
		}
	};
}