#ifndef RSS_ITEM_HPP
#define RSS_ITEM_HPP

#include <string>
class rss_item {

	public:
	std::string title, description, link;
	long date;
	void set_date(const std::string &input);
};
#endif
