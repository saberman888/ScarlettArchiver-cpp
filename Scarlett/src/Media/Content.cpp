#include "Scarlett/Media/Content.hpp"
#include "Scarlett/Internal/Exceptions.hpp"
#include "Scarlett/Internal/Helpers.hpp"
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <cpprest/http_client.h>
#include <cpprest/base_uri.h>
#include <fstream>
#include <regex>
#include <cstddef>


BOOST_CLASS_EXPORT_GUID(Scarlett::Media::Content, "ContentInfo");
BOOST_SERIALIZATION_SHARED_PTR(Scarlett::Media::Content);
template<class Archive>
void Scarlett::Media::Content::serialize(Archive& ar, const unsigned int version)
{
	using namespace boost::serialization;
	ar& make_nvp("ContentInfo", _ContentType);
	ar& make_nvp("ContentSize", _ContentSize);
	auto url = URL.to_string();
	ar& make_nvp("URL", url);
}
template void Scarlett::Media::Content::serialize<boost::archive::xml_oarchive>(boost::archive::xml_oarchive& ar, const unsigned int version);
template void Scarlett::Media::Content::serialize<boost::archive::xml_iarchive>(boost::archive::xml_iarchive& ar, const unsigned int version);


Scarlett::Media::Content::Content(const String& URL) : URL(URL){}
Scarlett::Media::Content::Content() = default;
Scarlett::Media::Content::Content(const URI& uri) : URL(uri){}
Scarlett::Media::Content::Content(const URI& uri, std::filesystem::path destination) : URL(uri), location(destination){}
Scarlett::Media::Content::~Content() = default;

const auto Scarlett::Media::Content::GetContent() { return Response->extract_vector().get(); }
const Scarlett::String Scarlett::Media::Content::GetStringContent(bool ignore_content_type) { return Response->extract_string(ignore_content_type).get(); }
const auto Scarlett::Media::Content::GetJSONContent() { return Response->extract_json().get(); }
const Scarlett::String Scarlett::Media::Content::GetURLString() { return URL.to_string(); }
void Scarlett::Media::Content::SetURL(const URI& uri) { URL = uri; }
const Scarlett::URI Scarlett::Media::Content::GetURL() { return URL; }
const std::filesystem::path Scarlett::Media::Content::GetPath() { return location; }
void Scarlett::Media::Content::SetPath(const std::filesystem::path destination) { location = destination; }
void Scarlett::Media::Content::Write(const Scarlett::String& filename)
{
	auto full_path = location / filename;
	Scarlett::ofstream out(full_path, std::ios::binary | std::ios::out);
	for (auto& data : GetContent())
		out << data;
}
const Scarlett::String Scarlett::Media::Content::ContentType()
{
	return _ContentType.empty() ? String() : _ContentType[0];
}
const Scarlett::Size Scarlett::Media::Content::ContentSize()
{
	return _ContentSize.get_value_or(0);
}

const Scarlett::StatusCode Scarlett::Media::Content::FetchContent(std::optional<URI> URL)
{
	Response.emplace(Download(URL.value_or(
		this->URL
	)));

	if (!Response->headers().empty()) {
		_ContentSize = Response->headers().content_length();
		_ContentType = splitString(
			Response->headers().content_type(),
			SCARLETT_WIDEN('/'));
	}

	return Response->status_code();
}

const Scarlett::String Scarlett::Media::Content::Extension()
{
	if (!_ContentType.empty())
	{
		if (std::regex_match(Scarlett::toString(_ContentType[1]), std::regex(";.+=.+")))
		{
			return splitString(_ContentType[1], SCARLETT_WIDEN(';'))[0];
		}
		else {
			return _ContentType[1];
		}
	}
	return String();
}
