#include "Scarlett/Reddit/BaseTypes/TextPost.hpp"
BOOST_SERIALIZATION_SHARED_PTR(Scarlett::Reddit::BaseTypes::TextPost);

namespace Scarlett::Reddit::BaseTypes
{
	TextPost::TextPost(const JsonValue& json) : Text("[deleted]"_u) {
		Thing::Read(json);
		Read(json);
	}

	bool TextPost::operator==(TextPost& other)
	{
		return Postable::operator==(other) && Text == other.Text;
	}

	bool TextPost::operator!=(TextPost& other)
	{
		return Thing::operator!=(other) && other.Text != Text;
	}

	void TextPost::Read(const JSON::value& json) {
		try {
			if (json.has_string_field("selftext"_u)) {
				Text = json.at("selftext"_u).as_string();
			}
			else if (json.has_string_field("body"_u)) {
				Text = json.at("body"_u).as_string();
			}
		}
		catch (JSON::json_exception& e) {
			scarlettNestedThrow("Failed to extract JSON from SelfPost, " + std::string(e.what()));
		}
	}
};