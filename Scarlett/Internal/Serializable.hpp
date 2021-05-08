#pragma once
#include <filesystem>
#include <type_traits>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include "exceptions.hpp"

namespace Scarlett::Internal
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

};
