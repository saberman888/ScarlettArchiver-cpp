#include "Scarlett/Internal/Serializable.hpp"
#include <boost/serialization/shared_ptr.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include "Scarlett/Internal/Exceptions.hpp"

#include "Scarlett/Reddit/Video.hpp"
#include "Scarlett/Reddit/SubredditMetadata.hpp"
#include "Scarlett/Reddit/SelfPost.hpp"
#include "Scarlett/Reddit/RedditStatistics.hpp"
#include "Scarlett/Reddit/Galleries.hpp"
#include "Scarlett/Reddit/Comment.hpp"
#include <type_traits>


namespace Scarlett
{
	template<typename T>
	void Serialize(const std::filesystem::path destination, T& sourceObj, const std::string Tag)
	{
		std::ofstream out;
		out.exceptions(std::ofstream::failbit | std::ofstream::badbit);
		try {
			out.open(destination);
			boost::archive::xml_oarchive xoa(out);
			if constexpr (std::is_pointer<T>::value) {
				xoa << boost::serialization::make_nvp(Tag.c_str(), *sourceObj);
			}
			else {
				xoa << boost::serialization::make_nvp(Tag.c_str(), sourceObj);
			}
		}
		catch (std::system_error& e) {
			scarlettNestedThrow(e.what());
		}
		catch (std::exception& e) {
			scarlettNestedThrow(e.what());
		}
	}

	template void Serialize<boost::shared_ptr<Reddit::Video>>(const std::filesystem::path destination, boost::shared_ptr<Reddit::Video>& sourceObj, const std::string Tag);
	template void Serialize<boost::shared_ptr<Reddit::SelfPost>>(const std::filesystem::path destination, boost::shared_ptr<Reddit::SelfPost>& sourceObj, const std::string Tag);
	template void Serialize<boost::shared_ptr<Reddit::Gallery>>(const std::filesystem::path destination, boost::shared_ptr<Reddit::Gallery>& sourceObj, const std::string Tag);
	template void Serialize<boost::shared_ptr<Reddit::Link>>(const std::filesystem::path destination, boost::shared_ptr<Reddit::Link>& sourceObj, const std::string Tag);
	template void Serialize<boost::shared_ptr<Reddit::Comment>>(const std::filesystem::path destination, boost::shared_ptr<Reddit::Comment>& sourceObj, const std::string Tag);
	template void Serialize<boost::shared_ptr<Reddit::SubredditMetadata>>(const std::filesystem::path destination, boost::shared_ptr<Reddit::SubredditMetadata>& sourceObj, const std::string Tag);
	template void Serialize<boost::shared_ptr<Reddit::RedditStatistics>>(const std::filesystem::path destination, boost::shared_ptr<Reddit::RedditStatistics>& sourceObj, const std::string Tag);
	template void Serialize<Reddit::SubredditMetadata>(const std::filesystem::path destination, Reddit::SubredditMetadata& sourceObj, const std::string Tag);
	template void Serialize<Reddit::RedditStatistics>(const std::filesystem::path destination, Reddit::RedditStatistics& sourceObj, const std::string Tag);
	template void Serialize<std::unique_ptr<Reddit::SubredditMetadata>>(const std::filesystem::path destination, std::unique_ptr<Reddit::SubredditMetadata>& sourceObj, const std::string Tag);






	template<typename T>
	T DeSerialize(const std::filesystem::path source, const std::string Tag)
	{

		std::ifstream in;
		in.exceptions(std::ofstream::failbit | std::ofstream::badbit);
		try {
			in.open(source);
			boost::archive::xml_iarchive xia(in);

			if constexpr (std::is_pointer<T>::value)
			{
				// Use a simpler type without any pointers
				using simpleT = typename std::remove_pointer<T>::type;

				// Allocate a new instance, deserialize and return
				simpleT* temp = new simpleT;
				xia >> boost::serialization::make_nvp(Tag.c_str(), *temp);
				return temp;
			}
			else {
				T temp;
				xia >> boost::serialization::make_nvp(Tag.c_str(), temp);
				return temp;
			}

		}
		catch (std::system_error& e) {
			scarlettNestedThrow(e.what());
		}
		catch (std::exception& e) {
			scarlettNestedThrow(e.what());
		}
	}


	template Reddit::Video* DeSerialize(const std::filesystem::path source, const std::string Tag);
	template Reddit::Comment* DeSerialize(const std::filesystem::path source, const std::string Tag);
	template Reddit::Gallery* DeSerialize(const std::filesystem::path source, const std::string Tag);
	template Reddit::Link* DeSerialize(const std::filesystem::path source, const std::string Tag);
	template Reddit::SelfPost* DeSerialize(const std::filesystem::path source, const std::string Tag);
	template Reddit::RedditStatistics* DeSerialize(const std::filesystem::path source, const std::string Tag);
	template Reddit::SubredditMetadata* DeSerialize(const std::filesystem::path source, const std::string Tag);
};