#include "Scarlett/Media/Content.hpp"
BOOST_CLASS_EXPORT_GUID(Scarlett::Media::Content, "ContentInfo");
BOOST_SERIALIZATION_SHARED_PTR(Scarlett::Media::Content);

Scarlett::Media::Content::Content(const String& URL)
{
	try {
		this->URL = URI(URL);
	}
	catch (const std::exception&) {
		throw;
	}
}

Scarlett::StatusCode Scarlett::Media::Content::FetchContent(std::optional<URI> URL)
{
	Response.emplace(Download(URL.value_or(
		this->URL
	)));

	if (!Response->headers().empty()) {
		_ContentSize = Response->headers().content_length();
		_ContentType = splitString(
			Response->headers().content_type(),
			WIDEN('/'));
	}

	return Response->status_code();
}

Scarlett::String Scarlett::Media::Content::Extension()
{
	if (!_ContentType.empty())
	{
		if (std::regex_match(Scarlett::toString(_ContentType[1]), std::regex(";.+=.+")))
		{
			return splitString(_ContentType[1], WIDEN(';'))[0];
		}
		else {
			return _ContentType[1];
		}
	}
	return String();
}