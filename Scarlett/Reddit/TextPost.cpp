#include "TextPost.hpp"
BOOST_CLASS_EXPORT(ScarlettArchiver::RedditAsset::SelfPost);


namespace ScarlettArchiver::RedditAsset
{
	TextPost::TextPost(const JSON::value& json) : Text("[deleted]") {
		Postable::Read(json);
		Read(json);
	}

	bool TextPost::operator==(TextPost& other)
	{
		return Postable::operator==(other) && Text == other.Text;
	}

	bool TextPost::operator!=(TextPost& other)
	{
		return Postable::operator!=(other) && other.Text != Text;
	}

	void TextPost::Read(const JSON::value& json) {
		try {
			if (json.has_string_field(L"selftext")) {
				Text = ToU8String(json.at(L"selftext").as_string());
			}
			else if (json.has_string_field(L"body")) {
				Text = ToU8String(json.at(L"body").as_string());
			}
		}
		catch (JSON::json_exception& e) {
			scarlettNestedThrow("Failed to extract JSON from SelfPost, " + std::string(e.what()));
		}
	}

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