#include "TextPost.hpp"
BOOST_CLASS_EXPORT(ScarlettArchiver::RedditAsset::SelfPost);


namespace ScarlettArchiver::RedditAsset
{
	TextPost::TextPost(const nlohmann::json& json) : Text("[deleted]") {
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

	void TextPost::Read(const nlohmann::json& json) {
		try {
			if (json.contains("selftext")) {
				json.at("selftext").get_to(Text);
			}
			else if (json.contains("body")) {
				json.at("body").get_to(Text);
			}
		}
		catch (nlohmann::json::exception& e) {
			scarlettNestedThrow("Failed to extract JSON from SelfPost, " + std::string(e.what()));
		}
	}

	SelfPost::SelfPost(const nlohmann::json& json) : TextPost(json)
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