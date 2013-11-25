#ifndef RSS_ITEM_HPP
#define RSS_ITEM_HPP

#include <string>
class rss_item {

	public:
	std::wstring title, description, link;
	long date;
	void set_date(const std::wstring &input);
};
#endif
