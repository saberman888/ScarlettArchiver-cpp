#pragma once
#include <string>
#include <tinyxml2.h>

namespace Scarlett::Media 
{
	/*
		Some information here might disapear or more might get added. I'm still learning this stuff.
	*/
	typedef struct __vidinfo
	{
		std::string BaseURL,
			mimeType,
			schemeIdUri,
			codecs,
			Bandwidth;

		int Width{ 0 },
			Height{ 0 },
			samplingRate{ 0 };

		void Read(const tinyxml2::XMLElement* Representation);
	}VideoInfo;
}
