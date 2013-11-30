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

	//Convert to map
	std::map<unsigned, rss_item> item_map;

	//TODO make rss_parser return a map in the first place.
	for (unsigned i = 0; i < items.size(); i++) {
		item_map[i] = items[i];
	}

	window.add_items(item_map);

	auto cb = std::bind(&rss_gui::add_path, &window, std::placeholders::_1, std::placeholders::_2);

	std::thread t(rss_grabber::process_img_list, item_map, std::move(cb)); t.detach();

	window.show();
	return app.exec();
}
