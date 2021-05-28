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
			if (std::filesystem::exists("accessdata.xml"))
			{
				tinyxml2::XMLDocument d("accessdata.xml");

				tinyxml2::XMLElement* root = d.FirstChildElement("accessdata");

				acd.client_key = root->FirstChildElement("client_key")->GetText();
				acd.client_secret = root->FirstChildElement("client_secret")->GetText();
				acd.redirect_uri = root->FirstChildElement("redirect_uri")->GetText();
				acd.useragent = root->FirstChildElement("useragent")->GetText();
				acd.username = root->FirstChildElement("username")->GetText();
				acd.password = root->FirstChildElement("password")->GetText();
			}
			else {
				throw std::runtime_error("Fatal Error: accessdata.xml is not present!");
			}
		}
	};
}