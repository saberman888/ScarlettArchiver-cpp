#include "Scarlett/Reddit/BaseTypes/Link.hpp"
#include "Scarlett/Media/Imgur.hpp"
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
BOOST_SERIALIZATION_SHARED_PTR(Scarlett::Reddit::Link)
BOOST_CLASS_EXPORT_GUID(Scarlett::Reddit::Link, "Link")


namespace Scarlett
{

    namespace Reddit
    {
        template<class Archive>
        void Link::serialize(Archive& ar, const unsigned int version)
        {
                using namespace boost::serialization;
                ar& make_nvp("Thing", base_object<Thing>(*this));
                ar& make_nvp("Domain", Domain);
                ar& make_nvp("Title", Title);
                ar& make_nvp("Hint", Hint);
        }


        template void Link::serialize<boost::archive::xml_oarchive>(boost::archive::xml_oarchive& ar, const unsigned int version);
        template void Link::serialize<boost::archive::xml_iarchive>(boost::archive::xml_iarchive& ar, const unsigned int version);

        Link::Link() = default;
        Link::Link(const JsonValue& json, std::optional<String> ImgurClientId) : ImgurClientId(ImgurClientId), Thing(json)
        {}

        const String Link::getDomain() { return Domain; }
        const String Link::getTitle() { return Title; }
        const Media::Content Link::getURL() { return URL; }

        String Link::GetContent() {
            using namespace Scarlett::Media;

            log->info("{}: Getting Image", toString(Id));
            if (ImgurClientId) {
                log->info("{}: Resolving the URL through the Imgur API", toString(Id));
                return ImgurAccess::GetImage(URL.GetURLString(),
                    ImgurClientId.value());
            }
            else {
                log->info("{]: Image returned", toString(Id));
                return URL.GetURLString();
            }
        }

        bool Link::operator==(Link& other)
        {
            return Thing::operator==(other);
        }

        bool Link::operator!=(Link& other)
        {
            return Thing::operator!=(other);
        }

        void Link::Read(const JsonValue& json)
        {
            try {
                if (json.has_string_field("post_hint"_u)) {
                    Hint = json.at("post_hint"_u).as_string();
                }
            }
            catch (const JSON::json_exception& e) {
                scarlettNestedThrow(e.what());
            }
        }
    }

    template class Scarlett::Serializable<Reddit::Link>;
}


