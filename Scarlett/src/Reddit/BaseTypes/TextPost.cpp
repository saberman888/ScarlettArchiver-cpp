#include "Scarlett/Reddit/BaseTypes/TextPost.hpp"
#include "Scarlett/Internal/Exceptions.hpp"
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>
BOOST_SERIALIZATION_SHARED_PTR(Scarlett::Reddit::TextPost)
BOOST_CLASS_EXPORT_GUID(Scarlett::Reddit::TextPost, "TextPost")

namespace Scarlett
{
    namespace Reddit
    {
        template<class Archive>
        void TextPost::serialize(Archive& ar, const unsigned int version)
        {
            using namespace boost::serialization;
            ar& make_nvp("Thing", base_object<Thing>(*this));
            ar& make_nvp("Text", Text);
        }
        template void TextPost::serialize<boost::archive::xml_oarchive>(boost::archive::xml_oarchive& ar, const unsigned int version);
        template void TextPost::serialize<boost::archive::xml_iarchive>(boost::archive::xml_iarchive& ar, const unsigned int version);

        TextPost::TextPost() = default;

        TextPost::TextPost(const JsonValue& json) : Text("[deleted]"_u), Thing(json) {
            Read(json);
        }

        bool TextPost::operator==(TextPost& other)
        {
            return Thing::operator==(other) && Text == other.Text;
        }

        bool TextPost::operator!=(TextPost& other)
        {
            return Thing::operator!=(other) && other.Text != Text;
        }

        const String TextPost::getText() { return Text; }

        void TextPost::Read(const JsonValue& json) {
            try {
                if (json.has_string_field("selftext"_u)) {
                    Text = json.at("selftext"_u).as_string();
                }
                else if (json.has_string_field("body"_u)) {
                    Text = json.at("body"_u).as_string();
                }
            }
            catch (JSON::json_exception& e) {
                scarlettNestedThrow("Failed to extract JSON from TextPost, " + std::string(e.what()));
            }
        }

        bool IsSelf(const JsonValue& val)
        {
            return (val.has_string_field("is_self"_u) && val.at("is_self"_u).as_bool());
        }
    }

    template class Scarlett::Serializable<Reddit::TextPost>;
};
