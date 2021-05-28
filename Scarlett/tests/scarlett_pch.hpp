#pragma once

#include "Scarlett/Scarlett.hpp"
#include "gtest/gtest.h"
#include "tinyxml2.h"
#include <filesystem>
#include <string>
#include <exception>

namespace Scarlett::Tests
{
	class TestAccessData : public ::testing::Test
	{
	protected:
		std::string ClientId, Secret, RedirectUri, UserAgent, Username, Password;

		TestAccessData()
		{
			if (std::filesystem::exists("accessdata.xml"))
			{
				tinyxml2::XMLDocument d("accessdata.xml");

				tinyxml2::XMLElement* root = doc.FirstChildElement("accessdata");

				ClientId = root->GetFirstChildElement("ClientId")->GetText();
				Secret = root->GetFirstChildElement("Secret")->GetText();
				RedirectUri = root->GetFirstChildElement("RedirectUri")->GetText();
				UserAgent = root->GetFirstChildElement("UserAgent")->GetText();
				Username = root->GetFirstChildElement("Username")->GetText();
				Password = root->GetFirstChildElement("Password")->GetText();
			}
			else {
				throw std::runtime_error("Fatal Error: accessdata.xml is not present!");
			}
		}
	};
}