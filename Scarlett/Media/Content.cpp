#include "Content.hpp"
BOOST_CLASS_EXPORT_GUID(Scarlett::Media::Content, "ContentInfo");

Scarlett::Media::Content::Content(const std::string& URL)
{
	try {
		this->URL = ScarlettURL(
			utility::conversions::to_string_t(URL)
		);
	}
	catch (const std::exception& e) {
		throw;
	}
}

Scarlett::StatusCode Scarlett::Media::Content::FetchContent(std::optional<std::string> URL)
{
	Response.emplace(Download(URL.value_or(
		u8(this->URL.to_string())
	)));

	if (!Response->headers().empty()) {
		_ContentSize = Response->headers().content_length();
		_ContentType = splitString(
			u8(Response->headers().content_type()),
			'/');
	}

	return Response->status_code();
}

std::string Scarlett::Media::Content::Extension()
{
	if (!_ContentType.empty())
	{
		if (std::regex_match(_ContentType[1], std::regex(";.+=.+")))
		{
			return splitString(_ContentType[1], ';')[0];
		}
		else {
			return _ContentType[1];
		}
	}
	return std::string();
}

std::string Scarlett::Media::Content::ContentType()
{
	return _ContentType.empty() ? std::string() : _ContentType[0];
}

Scarlett::Size Scarlett::Media::Content::ContentSize()
{
	return _ContentSize.get_value_or(0);
}
