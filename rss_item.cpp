#include "rss_item.hpp"
#include <ctime>
#include <cstring>

void rss_item::set_date(const std::wstring &input) {
	
	static const char *format("%d %h %Y %H:%M:%S %Z");
	//Mon, 18 Nov 2013 09:46:11 GMT

	struct tm tm;
	memset(&tm, 0, sizeof(tm));

	//We know this is ascii. If it's not it will fail anyway.
	strptime((const char *)input.c_str() + 5, format, &tm);
	date = (long)mktime(&tm);
}
