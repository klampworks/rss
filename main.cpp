#include <vector>
#include "rss_gui.hpp"
#include <QApplication>
#include "rss_parser.hpp"
#include "rss_grabber.hpp"

#include <iostream>
int main(int argc, char **argv) {

	QApplication app(argc, argv);
	rss_gui window;

	std::vector<rss_item> items;

	if (argc < 2) {
		
		std::cout << "Pass the url of the RSS feed as the first argument." << std::endl;
		
		items = rss_parser::parse_file("rss.xml");
	} else {

		//This is ok until we have a settings window.
		const char *url = argv[1];

		std::string xml = rss_grabber::grab_xml(url);
		items = rss_parser::parse_xml(xml);
	}


	for (const auto &item : items) {
		window.add_item(item.title);
	}

	window.show();
	return app.exec();
}
