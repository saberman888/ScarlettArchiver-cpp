#pragma once

#include <memory>
#include <optional>
#include "../Internal/Types.hpp"
#include "BaseTypes/TextPost.hpp"
#include <boost/serialization/optional.hpp>

namespace Scarlett::Reddit
{
	
    class SCDLL Comment : public TextPost
	{
	public:
		Comment(const String& ParentId);
		Comment(const JsonValue& json, boost::optional<String> ParentId = boost::none);
		std::vector<std::unique_ptr<Comment>> replies; 
		void GetRedditComments();

	private:
		Comment();
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version);

		boost::optional<String> ParentId{ boost::none };
		void Read(const JsonValue& json);
	};	
	using CommentTree = std::vector<std::unique_ptr<Comment>>;
};
