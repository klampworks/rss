#ifndef RSS_GRABBER_HPP
#define RSS_GRABBER_HPP

#include <curl/curl.h>
#include <curl/easy.h>
#include <string>

namespace rss_grabber {

	std::string grab_xml(const char *url);
}
#endif
