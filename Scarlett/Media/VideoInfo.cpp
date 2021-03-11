#include "Media/VideoInfo.hpp"

namespace Scarlett::Media
{
	void __vidinfo::Read(const tinyxml2::XMLElement* Representation)
	{
		codecs = Representation->Attribute("codecs");

		if (mimeType == "audio/mp4") {
			samplingRate = std::stoi(Representation->Attribute("audioSamplingRate"));
			schemeIdUri = Representation->FirstChildElement("AudioChannelConfiguration")->Attribute("schemeIdUri");
		}
		else if (mimeType == "video/mp4") {
			Height = std::stoi(Representation->Attribute("height"));
			Width = std::stoi(Representation->Attribute("width"));
			frameRate = std::stoi(Representation->Attribute("frameRate"));
		}

		Bandwidth = Representation->Attribute("bandwidth");
		BaseURL = Representation->FirstChildElement("BaseURL")->Value();

	}
};