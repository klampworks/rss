#include "rss_gui.hpp"
#include <QDesktopWidget>
#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPalette>
#include <QScrollArea>
#include <QScrollBar>
#include "rss_grabber.hpp"
#include "rss_parser.hpp"
#include <thread>


rss_gui::rss_gui(std::string url_p, QWidget *parent) : 
	fragment(parent), 
	desc_window(), 
	sigmap(this),
	url(url_p) {

	setMaximumHeight(200);
	setFixedHeight(200);

	//Add a layout to the main window.
	this->setLayout(&base_layout);

	//Create a scrollarea, add it to the main window layout
	s.setWidgetResizable(true); //this is fucking important.

	//Make the scrollarea transparent and borderless.
	QPalette pal;
	pal.setColor(QPalette::Window, Qt::transparent);
	s.setPalette(pal);
	s.setFrameShape(QFrame::NoFrame);

	base_layout.addWidget(&s);

	//Create a plane for the scrollable contents.

	//Create a layout for the conents.
	v_layout.setSpacing(0);
	contents.setLayout(&v_layout);
	contents.setContentsMargins(5,5,15,5);

	s.setWidget(&contents);
	s.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	pal.setColor(QPalette::Window, Qt::transparent);
	s.verticalScrollBar()->setAutoFillBackground(true);

	item_font.setPointSize(8);
	item_bg.setColor(QPalette::Button, Qt::transparent);

	QObject::connect(&sigmap, SIGNAL(mapped(int)), this, SLOT(open_desc(int)));

	connect(&update_timer, SIGNAL(timeout()), this, SLOT(update()));
//	update_timer.start(1000);
}

//Needs to be a copy, unfortunantly.
void rss_gui::add_items(std::map<unsigned, rss_item> item_map) {

	//TODO: Used a fixed length list and handle updates correctly.
	item_list = item_map;

	while (items.size()) {
		v_layout.removeWidget(items.back());
		delete items.back();
		items.pop_back();
	}

	for (const auto &item : item_list) { 

		QPushButton *b = new QPushButton(QString::fromWCharArray(item.second.title.c_str()));
		b->setFlat(true);
		b->setFont(item_font);
		b->setPalette(item_bg);
		QObject::connect(b, SIGNAL(clicked()), &sigmap, SLOT(map()));
		sigmap.setMapping(b, item.first);

		v_layout.addWidget(b);
		b->show();

		items.push_back(b);
	}
}

void rss_gui::add_path(unsigned index, std::string &&path) {
	item_list[index].path = path;
}

void rss_gui::open_desc(int i) {

	//Avoid multiple hash lookups.
	rss_item *working_item = &item_list[i];

	desc_window.add_desc(QString::fromWCharArray(working_item->description.c_str()));
	desc_window.set_image(working_item->path);

	if (desc_window.isVisible()) {
		desc_window.hide();
	} else {
		desc_window.show();
		desc_window.offset = width() + 10;
	}
}

void rss_gui::update() {

	std::vector<rss_item> items;
	std::string xml = rss_grabber::grab_xml(url.c_str());
	items = rss_parser::parse_xml(xml);

	//Convert to map
	std::map<unsigned, rss_item> item_map;

	//TODO make rss_parser return a map in the first place.
	for (unsigned i = 0; i < items.size(); i++) {
		item_map[i] = items[i];
	}

	add_items(item_map);

	auto cb = std::bind(&rss_gui::add_path, this, std::placeholders::_1, std::placeholders::_2);
	std::thread t(rss_grabber::process_img_list, item_map, std::move(cb)); t.detach();
}
