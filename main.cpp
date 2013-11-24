#include <vector>

#include "rss_gui.hpp"
#include <QApplication>
#include "rss_parser.hpp"

int main(int argc, char **argv) {

	QApplication app(argc, argv);
	rss_gui window;

	std::vector<rss_item> items = rss_parser::parse_file("rss.xml");

	for (const auto &item : items) {
		window.add_item(item.title);
	}

	window.show();
	return app.exec();
}
