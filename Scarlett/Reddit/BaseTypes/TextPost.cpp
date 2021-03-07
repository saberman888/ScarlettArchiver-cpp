#include "TextPost.hpp"



namespace Scarlett::Reddit::BaseTypes
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
			if (json.has_string_field("selftext"_u)) {
				Text = ToU8String(json.at("selftext"_u).as_string());
			}
			else if (json.has_string_field("body"_u)) {
				Text = ToU8String(json.at("body"_u).as_string());
			}
		}
		catch (JSON::json_exception& e) {
			scarlettNestedThrow("Failed to extract JSON from SelfPost, " + std::string(e.what()));
		}
	}
};