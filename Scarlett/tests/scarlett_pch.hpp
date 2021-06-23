#pragma once

#include "Scarlett/Scarlett.hpp"
#include "gtest/gtest.h"
#include "tinyxml2.h"
#include <filesystem>
#include <string>
#include <exception>
#include <iosfwd>

namespace Scarlett::Tests
{
	class TestAccessData : public ::testing::Test
	{
	protected:
		Scarlett::Client::AccessData acd;

		TestAccessData()
		{
		}

		inline struct Scarlett::Client::AccessData load_configuration(const std::string filename)
		{
			tinyxml2::XMLDocument d;
			struct Client::AccessData acd;
			if(d.LoadFile(filename.c_str()) == tinyxml2::XML_SUCCESS)
			{
				auto root = d.RootElement();

				acd.client_key = TestAccessData::get_element(root, "client_key");
				acd.client_secret = TestAccessData::get_element(root, "client_secret");
				acd.useragent = TestAccessData::get_element(root, "useragent");
				acd.redirect_uri = TestAccessData::get_element(root, "redirect_uri");
				acd.username = TestAccessData::get_element(root, "username");
				acd.password = TestAccessData::get_element(root, "password");
			} else {
				throw std::runtime_error("Fatal Error: Could not parse " + filename);
			}
			return acd;
		}
	private:
		inline bool try_get_element(tinyxml2::XMLElement* parent, const std::string& tag, std::string& dest)
		{
			auto elem = parent->FirstChildElement(tag.c_str());
			if(elem)
			{
				if(elem->GetText())
				{
					dest = std::string(elem->GetText());
					return true;
				} 
				return false;
			} else {
				return false;
			}
		}
		inline std::string get_element(tinyxml2::XMLElement* parent, const std::string& tag)
		{
			std::string dest;
			auto result = try_get_element(parent, tag, dest);
			if(!result)
			{
				std::cout << "Warning: " << tag <<" was not present or did not exist" << std::endl;
			}
			return dest;
		}
	};
}
