#include "rss_gui.hpp"
#include <QApplication>
#include <iostream>


int main(int argc, char **argv) {

	QApplication app(argc, argv);

	std::string url("");

	if (argc < 2) {
		std::cout << "Pass the url of the RSS feed as the first argument." << std::endl;

		// For debugging, no arguments will load from a cached RSS file.
		//exit(1);
	} else {
		url = argv[1];
	}

	rss_gui window(url, QColor(11, 11, 44, 200));

	window.show();
	return app.exec();
}
