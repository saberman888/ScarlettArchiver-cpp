#include "Scarlett/Reddit/Galleries.hpp"
BOOST_SERIALIZATION_SHARED_PTR(Scarlett::Reddit::Gallery);
BOOST_CLASS_EXPORT_GUID(Scarlett::Reddit::Gallery, "Gallery")


namespace Scarlett::Reddit
{

	template<class Archive>
	void Gallery::serialize(Archive& ar, const unsigned int version)
	{
		using namespace boost::serialization;
		using namespace boost::serialization;
		ar& make_nvp("Link", base_object<Link>(*this));
		ar& make_nvp("Images", Images);
	}

	template void Gallery::serialize<boost::archive::xml_oarchive>(boost::archive::xml_oarchive& ar, const unsigned int version);
	template void Gallery::serialize<boost::archive::xml_iarchive>(boost::archive::xml_iarchive& ar, const unsigned int version);

	Gallery::Gallery() = default;
	const std::vector<Media::Content> Gallery::GetImages() { return Images;  }
	Gallery::Gallery(const JSON::value& json, const std::optional<String> ImgurClientId) : Link(json, ImgurClientId)	{
		Read(json);	

		// Since we have all the data, resolve the album if it is an imgur one
		if (Media::ImgurAccess::IsImgurLink(URL.GetURLString()) && ImgurClientId)
		{
			for (auto& urls : Media::ImgurAccess::GetAlbum(URL.GetURLString(), ImgurClientId.value()))
			{
				Images.push_back(urls);
			}
		}
	}


	void Gallery::Read(const JSON::value& json)
	{
		if (!ImgurClientId) {
			for (auto& image : json.at("gallery_data"_u).at("items"_u).as_array())
			{
				try {
					auto mediaId = image.at("media_id"_u).as_string();
					auto mediaMetadata = json.at("media_metadata"_u).at(mediaId).at("m"_u).as_string();

					String imageExtension = Scarlett::splitString(
						mediaMetadata,
						SCARLETT_WIDEN('/')
					)[1];

					Images.push_back(Media::Content(
						"https://i.redd.it/"_u + mediaId + "."_u + imageExtension
					));
				}
				catch (JSON::json_exception& e) {
					scarlettNestedThrow("Failed to parse JSON for Gallery, " + std::string(e.what()));
				}
			}
		}
	}

	bool Gallery::IsGallery(const JSON::value& json)
	{
		if ((json.has_boolean_field("is_gallery"_u) && json.at("is_gallery"_u).as_bool() && json.has_field("gallery_data"_u)) ||
			(Media::ImgurAccess::IsAlbum(json.at("url"_u).as_string())));
			return true;
		return false;
	}


	
};


