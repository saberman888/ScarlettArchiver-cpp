#pragma once

#include <memory>
#include <optional>
#include "Scarlett/Misc.hpp"
#include "BaseTypes/TextPost.hpp"
#include <boost/serialization/export.hpp>
namespace Scarlett::RedditAsset
{
	
	class Comment : public BaseTypes::TextPost
	{
	public:
		Comment(const std::string& ParentId);
		Comment(const JSON::value& json, std::optional<std::string> ParentId = std::nullopt);
		std::vector<std::unique_ptr<Comment>> replies; 
		void GetRedditComments();
		void GetPushShiftComments();

	private:
		Comment(){}
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& boost::serialization::base_object<TextPost>(*this);
			std::string pidName = ParentId.value_or("(null)");
			ar& pidName;
		}

		std::optional<std::string> ParentId;
		void Read(const JSON::value& json);
	};	
	using CommentTree = std::vector<std::unique_ptr<Comment>>;
};
