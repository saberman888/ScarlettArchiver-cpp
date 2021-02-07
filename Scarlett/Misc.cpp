#include "Misc.hpp"

namespace ScarlettArchiver
{
	char* strptime(const char* s, const char* f, struct tm* tm) {
		std::istringstream input(s);
		input.imbue(std::locale(setlocale(LC_ALL, nullptr)));
		input >> std::get_time(tm, f);
		if (input.fail()) {
			return nullptr;
		}
		return (char*)(s + input.tellg());
	}
	
	State Download(const std::string URL)
	{
		BasicRequest handle;
		handle.Setup(URL);
		handle.SetOpt(CURLOPT_FOLLOWLOCATION, 1L);
		State result = handle.SendRequest();
		handle.Cleanup();
		return result;
	}

	void Write(const nlohmann::json& src, const std::filesystem::path dest, const std::string filename)
	{
		std::filesystem::create_directories(dest);
		std::ofstream out(dest.string() + "/" + filename, std::ios::out);
		out << src.dump(4);
	}	
	
	void Write(const std::string& buff, std::string filename)
	{
		std::ofstream out(filename, std::ios::out);
		out << buff;
	}
}