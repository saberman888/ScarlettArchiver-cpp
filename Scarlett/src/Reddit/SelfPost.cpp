#include "Scarlett/Reddit/SelfPost.hpp"
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
BOOST_SERIALIZATION_SHARED_PTR(Scarlett::Reddit::SelfPost);
BOOST_CLASS_EXPORT_GUID(Scarlett::Reddit::SelfPost, "SelfPost");

namespace Scarlett::Reddit
{
	template<class Archive>
	void SelfPost::serialize(Archive& ar, const unsigned int version)
	{
		using namespace boost::serialization;
		ar& make_nvp("TextPost", base_object<TextPost>(*this));
	}
	template void SelfPost::serialize<boost::archive::xml_oarchive>(boost::archive::xml_oarchive& ar, const unsigned int version);
	template void SelfPost::serialize<boost::archive::xml_iarchive>(boost::archive::xml_iarchive& ar, const unsigned int version);

	SelfPost::SelfPost(const JSON::value& json) : TextPost(json)
	{
		TextPost::Read(json);
	}

	bool SelfPost::IsSelfPost(const JsonValue& json)
	{
		return (json.has_string_field("is_self"_u) && json.at("is_self"_u).as_bool());
	}

	SelfPost::SelfPost() = default;

};
