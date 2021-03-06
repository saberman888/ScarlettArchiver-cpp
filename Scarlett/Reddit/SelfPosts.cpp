#include "SelfPost.hpp"
BOOST_CLASS_EXPORT(ScarlettArchiver::RedditAsset::SelfPost);

namespace ScarlettArchiver::RedditAsset
{
	SelfPost::SelfPost(const JSON::value& json) : TextPost(json)
	{
		Linkable::Read(json);
		TextPost::Read(json);
	}

	bool SelfPost::operator==(SelfPost& other)
	{
		return TextPost::operator==(other) && Linkable::operator==(other);
	}

	bool SelfPost::operator!=(SelfPost& other)
	{
		return TextPost::operator!=(other) && Linkable::operator!=(other);
	}

};