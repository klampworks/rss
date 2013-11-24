#include <vector>
#include "rss_gui.hpp"
#include <QApplication>
#include "rss_parser.hpp"
#include "rss_grabber.hpp"

#include <iostream>
int main(int argc, char **argv) {

	QApplication app(argc, argv);
	rss_gui window;

	std::string xml = rss_grabber::grab_xml("http://www.sankakucomplex.com/feed/");
	std::cout << xml << std::endl;
	std::vector<rss_item> items = rss_parser::parse_xml(xml);

	for (const auto &item : items) {
		window.add_item(item.title);
	}

	window.show();
	return app.exec();
}
