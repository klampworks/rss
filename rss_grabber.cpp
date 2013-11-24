#include "rss_grabber.hpp"
#include <sstream>

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
}
