#include "Scarlett/Reddit/Comment.hpp"
#include "Scarlett/Internal/Exceptions.hpp"
#include "Scarlett/Internal/Helpers.hpp"
#include <boost/serialization/export.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/serialization/shared_ptr.hpp>
BOOST_SERIALIZATION_SHARED_PTR(Scarlett::Reddit::Comment)
BOOST_CLASS_EXPORT_GUID(Scarlett::Reddit::Comment, "Comment")

namespace Scarlett::Reddit
{
	template<class Archive>
	void Comment::serialize(Archive& ar, const unsigned int version)
	{
		using namespace boost::serialization;
		ar& make_nvp("TextPost", base_object<TextPost>(*this));
		ar& make_nvp("parent_id", ParentId);
	}
	template void Comment::serialize<boost::archive::xml_oarchive>(boost::archive::xml_oarchive& ar, const unsigned int version);
	template void Comment::serialize<boost::archive::xml_iarchive>(boost::archive::xml_iarchive& ar, const unsigned int version);

	Comment::Comment() = default;

    Comment::Comment(const String& ParentId)
    {
		this->ParentId.emplace(ParentId);
    }
	Comment::Comment(const JsonValue& json, boost::optional<String> ParentId)
	{
		TextPost::Read(json);
	}

    void Comment::GetRedditComments()
    {
		auto data = Download("https://reddit.com/"_u + ParentId.value() + ".json"_u);
		if (data.status_code() == 200)
		{
			try {
				Read(data.extract_json().get().at(1));
			}
			catch (JSON::json_exception& e) {
				scarlettNestedThrow("Failed to parse JSON for Comment, " + std::string(e.what()));
			}
		}
		else {
			scarlettThrow("Failed to gather additional comments!, " + std::to_string(data.status_code()));
		}
    }


	void Comment::Read(const JSON::value& json)
	{
		for (auto elem : json.at("data"_u).at("children"_u).as_array())
		{
			try {

				auto com = elem.at("data"_u);
				// If it has an array called children, it's probably a 'more children' object, or in other words keys to more comments
				if (com.has_field("children"_u) && com.at("children"_u).is_array())
				{
					// TODO: Implement Reddit API components to use more children object
					continue;
				}

				auto tempComment = std::make_unique<Comment>(com, ParentId);

				if (auto replies = com.at("replies"_u); replies.is_array() && replies.as_array().size() > 0) {
					auto innerchildren = com.at("replies"_u).at("data"_u).at("children"_u);
				}

				replies.push_back(std::move(tempComment));
			}
			catch (JSON::json_exception& e) {
				scarlettNestedThrow("Failed to parse comment JSON from CommentListing, " + toString(ParentId.value_or("???"_u)) + ", " + std::string(e.what()));
			}
		}
	}
};
