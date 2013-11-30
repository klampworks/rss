#ifndef RSS_GRABBER_HPP
#define RSS_GRABBER_HPP

#include <curl/curl.h>
#include <curl/easy.h>
#include <string>
#include <vector>
#include <map>
#include "rss_gui.hpp"

class rss_item;

namespace rss_grabber {

	void process_img_list(const std::map<unsigned, rss_item> &list, rss_gui *window);
	std::string grab_xml(const char *url);
}
#endif
