#include "TextPost.hpp"
BOOST_CLASS_EXPORT(ScarlettArchiver::RedditAsset::TextPost);


namespace ScarlettArchiver::RedditAsset
{
	TextPost::TextPost(const nlohmann::json& json) : Text("[deleted]") {
		Linkable::Read(json);
		Postable::Read(json);
		Read(json);
	}

	bool TextPost::operator==(TextPost& other)
	{
		return (Linkable::operator==(other) && Postable::operator==(other)) && Text == other.Text;
	}

	bool TextPost::operator!=(TextPost& other)
	{
		return (Linkable::operator!=(other) && Postable::operator!=(other)) && other.Text != Text;
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

};