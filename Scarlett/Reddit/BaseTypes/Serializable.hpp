#pragma once

namespace Scarlett
{
	class Serializable
	{
	public:
		virtual void Serialize(const std::filesystem::path destination) = 0;
		virtual void DeSerialize(const std::filesystem::path source) = 0;
	};
};