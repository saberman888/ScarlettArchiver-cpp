#pragma once

#include <string_view>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include "../../Internal/Helpers.hpp"

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
		String name;
		String id;
		Internal::Kind kind;
	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			using namespace boost::serialization;
			ar& make_nvp("name", name);
			ar& make_nvp("id", id);
			ar& make_nvp("kind", kind);
		}
	};
}