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
				tinyxml2::XMLDocument d;
				if(d.LoadFile("accessdata.xml") != tinyxml2::XML_SUCCESS)
				{
					throw std::runtime_error("Fatal Error: Failed to open accessdata.xml");
				}


				tinyxml2::XMLElement* root = d.FirstChildElement("accessdata");

				auto client_key = root->FirstChildElement("client_key");
				if(client_key)
				{
					if(client_key->GetText())
					{
						acd.client_key = client_key->GetText();
					} else {
						throw std::runtime_error("Fatal Error: client_key not found in accessdata.xml");
					}
				}

				auto client_secret = root->FirstChildElement("client_secret");
				if(client_secret)
				{
					if(client_secret->GetText())
					{
						acd.client_secret = client_secret->GetText();
					}
				}

				auto redirect_uri = root->FirstChildElement("redirect_uri");
				if(redirect_uri)
				{
					if(redirect_uri->GetText() == nullptr)
					{
						acd.redirect_uri = "http://localhost:3030";
					} else {
						acd.redirect_uri = redirect_uri->GetText();
					}
				}

				auto useragent = root->FirstChildElement("useragent");
				if(useragent)
				{
					acd.useragent = useragent->GetText();
				}

				auto username = root->FirstChildElement("username");
				if(username)
				{
					if(username->GetText())
					{
						acd.username = username->GetText();
					}
				}

				auto password = root->FirstChildElement("password");
				if(password)
				{
					if(password->GetText())
					{
						acd.password = password->GetText();
					}
				}
			}
			else {
				throw std::runtime_error("Fatal Error: accessdata.xml is not present!");
			}
		}
	};
}
