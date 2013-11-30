#include <vector>
#include "rss_gui.hpp"
#include <QApplication>
#include "rss_parser.hpp"
#include "rss_grabber.hpp"
#include <iostream>
#include <fstream>
#include <thread>
#include <functional>

int main(int argc, char **argv) {

	QApplication app(argc, argv);

	std::map<unsigned, rss_item> item_map;
	std::string url("");
	if (argc < 2) {
		
		std::cout << "Pass the url of the RSS feed as the first argument." << std::endl;
		
//		item_map = rss_parser::parse_file("rss.xml");
	} else {
		url = argv[1];
	}

	rss_gui window(url);

	window.show();
	return app.exec();
}
