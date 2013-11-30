#include "rss_grabber.hpp"
#include <sstream>
#include <fstream>
#include "rss_item.hpp"
#include <sys/stat.h>
#include <unistd.h>
#include "rss_parser.hpp"
#include <functional>
#include <utility>

namespace rss_grabber {

	CURL *curl;

	size_t write_data_string(char *ptr, size_t size, size_t nmemb, std::string *stream) {

		// What we will return
		int result = 0;

		// Is there anything in the buffer?
		if (stream != NULL) {

		    // Append the data to the buffer
		    stream->append(ptr, size * nmemb);
		    
		    // How much did we write?
		    result = size * nmemb;
		}

		return result;
	}

	std::string grab_xml(const char *url) {
					
		if (!curl) {
			//This is fine until we need extra options.
			curl = curl_easy_init();
			
			//Fucking cloudflare
			curl_easy_setopt(curl, CURLOPT_USERAGENT, 
				"Mozilla/5.0 (Windows; U; MSIE 9.0; Windows NT 9.0; en-US)");
		}
			
		std::string mi;

		curl_easy_setopt(curl, CURLOPT_URL, url);

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &mi);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_string);

		curl_easy_perform(curl);

		return mi;
	}

std::string process_img(std::string &&link) {

	//TODO: This refactoring is a bit vile....
	const char *path = "cache/";
	mkdir(path, 0777);

	std::string t_link, name, xml;
	std::ifstream ifs;

	auto prepare_link = [&link, &t_link, &name, &ifs, &path]() {
		t_link = link.substr(0, link.length() - 1);
		name = path + t_link.substr(t_link.find_last_of('/')+1);
		ifs.open(name);
	};

	auto save_page = [&xml, &ifs, &link, &name]() {

		xml = grab_xml(link.c_str());
		ifs.close();

		std::ofstream ofs(name);
		ofs << xml;
		ofs.close();
	};

	prepare_link();

	if (!ifs.good()) {

		//This page has not ben cached.
		save_page();

	} else {

		//This page has been chached, read it into a string.
		std::getline(ifs, xml, char(-1));
		ifs.close();

		if (xml.empty()) {
			unlink(name.c_str());
			//TODO: this could lead to infinite recursion.
			return process_img(std::move(link));
		}
	}
	
	link = rss_parser::parse_img(xml);
	prepare_link();

	if (!ifs.good()) {

		//grab the image and save it as this name.
		save_page();

	} //else we already have this image, nevermind.

	//Copy ellision.
	return std::move(name);
}

void process_img_list(const std::map<unsigned, rss_item> &list, 
	std::function<void(unsigned, std::string&&)> &&cb) {

	for (const auto &l: list) {

		std::string link;
		link.assign(l.second.link.begin(), l.second.link.end());
		cb(l.first, process_img(std::move(link)));
	}
}

}
