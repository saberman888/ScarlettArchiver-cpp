#pragma once

#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <ctime>
#include "../../Internal/Helpers.hpp"
#include "../../Internal/Exceptions.hpp"

namespace Scarlett::Reddit
{

	enum class Kind : int
	{
		Base,
		Link,
		Video,
		Gallery,
		Comment,
		SelfPost
	};

	class Thing
	{
	public:
		using Timestamp = time_t;

		inline bool operator==(const Thing& thing)
		{
			return (Name == thing.Name) && (Id == thing.Id) && (Author == thing.Author) && (Permalink == thing.Permalink) && (CreatedUTC == thing.CreatedUTC) && (CreatedLocal == thing.CreatedLocal);
		}

		inline bool operator!=(const Thing& thing)
		{
			return !this->operator==(thing);
		}

		inline const String getName() { return Name; }
		inline const String getId() { return Id; }
		inline const String getAuthor() { return Author; }
		inline const String getPermalink() { return Permalink; }
		inline const Timestamp getCreatedUTCTime() { return CreatedUTC; }
		inline const Timestamp getCreatedLocalTime() { return CreatedLocal; }
		virtual inline  const Kind getType() { return Kind::Base; }

	protected:
		Thing() {}
		Thing(const JsonValue& value) {
			Read(value);
		}

		String Name;
		String Id;
		String Author;
		String Permalink;
		Timestamp CreatedUTC{ 0L };
		Timestamp CreatedLocal{ 0L };

	private:
		virtual void Read(const JsonValue& value);

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			using namespace boost::serialization;
			ar& make_nvp("Name", Name);
			ar& make_nvp("Id", Id);
			ar& make_nvp("Permalink", Permalink);
			ar& make_nvp("Author", Author);
			ar& make_nvp("Created_UTC", CreatedUTC);
			ar& make_nvp("Created_Local", CreatedLocal);
		}
	};
}