#pragma once
#include "nlohmann/json.hpp"

namespace ScarlettArchiver
{
	class Retriever
	{
	public:
		virtual void Read(const nlohmann::json& json);
		virtual bool HasNext();
		virtual nlohmann::json Next();
	};
};
