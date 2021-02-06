#pragma once

#include <string>
#include "nlohmann/json.hpp"

class Linkable
{
public:
	std::string Domain, URL;
protected:
	virtual bool Read(const nlohmann::json data);
};
