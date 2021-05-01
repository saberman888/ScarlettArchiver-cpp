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
		in.exceptions(std::ofstream::failbit | std::ofstream::badbit);
		try {
			out.open(destination);
			boost::archive::xml_oarchive xoa(out);
			if constexpr (std::is_same<T, std::shared_ptr<T>>::value || std::is_same<T, std::unique_ptr<T>>::value || std::is_same<T, boost::shared_ptr<T>>::value)
			{
				xoa << boost::serialization::make_nvp(Tag.c_str(), *sourceObj.get());
			}
			else if constexpr (std::is_pointer<T>::value) {
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

			T temp;
			in.open(source);
			boost::archive::xml_iarchive xia(in);

			if constexpr (std::is_pointer<T>::value)
			{
				temp = new T;
				xia >> boost::serialization::make_nvp(Tag.c_str(), *temp);
			}
			else {
				xia >> boost::serialization::make_nvp(Tag.c_str(), temp);
			}
			return temp;

		}
		catch (std::system_error& e) {
			scarlettNestedThrow(e.what());
		}
		catch (std::exception& e) {
			scarlettNestedThrow(e.what());
		}
	}

};