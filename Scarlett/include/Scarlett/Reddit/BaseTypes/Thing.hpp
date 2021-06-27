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
		String Name;
		String Id;
		String Author;
		String Permalink;
		Internal::Kind kind;

		inline bool operator==(auto thing)
		{
			return (Name == thing.Name) && (Id == thing.Id) && (Author thing.Author) && (Permalink == thing.Permalink) && (kind == thing.kind);
		}

		inline bool operator!=(auto thing)
		{
			return !this->operator==(thing);
		}

	private:
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
		}
	};
}