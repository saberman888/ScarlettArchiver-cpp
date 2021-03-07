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
	
	web::http::http_response Download(const std::string URL)
	{
		web::http::client::http_client client(conv::to_string_t(URL));
		return client.request(web::http::methods::GET).get();
	}

	void Write(const JSON::value& src, const std::filesystem::path dest, const std::string filename)
	{
		// TODO: Reimplement with cpprestsdk json
		/*std::filesystem::create_directories(dest);
		std::ofstream out(dest.string() + "/" + filename, std::ios::out);
		out << src;*/
	}	
	
	void Write(const std::string& buff, std::string filename)
	{
		std::ofstream out(filename, std::ios::out);
		out << buff;
	}
}