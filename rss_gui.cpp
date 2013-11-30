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

rss_gui::rss_gui(std::string url_p, QColor bg_colour, QWidget *parent) : 
	fragment(bg_colour, parent), 
	desc_window(bg_colour), 
	sigmap(this),
	url(url_p) {

	setMaximumHeight(200);
	setFixedHeight(200);

	QPalette pal;
	pal.setColor(QPalette::Window, Qt::transparent);

	//Add a layout to the main window.
	this->setLayout(&base_layout);

	settings.setIcon(QPixmap(QString("settings.png")));
	settings.setFlat(true);
	settings.setPalette(pal);
	settings.setFixedSize(16, 16);

	refresh.setIcon(QPixmap(QString("refresh.png")));
	refresh.setFlat(true);
	refresh.setPalette(pal);
	refresh.setFixedSize(16, 16);
	connect(&refresh, SIGNAL(clicked()), this, SLOT(refresh_update()));

	topbar_title.setText("Title");
	topbar_title.setAlignment(Qt::AlignHCenter);

	topbar.addWidget(&topbar_title);
	topbar.addWidget(&refresh);
	topbar.addWidget(&settings);
	base_layout.addLayout(&topbar);

	//Create a scrollarea, add it to the main window layout
	s.setWidgetResizable(true); //this is fucking important.

	//Make the scrollarea transparent and borderless.
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

	if (url.empty()) {

		//This is just for debugging really.
		item_list = rss_parser::parse_file("rss.xml");
		update_items();
		auto cb = std::bind(&rss_gui::add_path, this, std::placeholders::_1, std::placeholders::_2);
		std::thread t(rss_grabber::process_img_list, item_list, std::move(cb)); t.detach();
	} else {
		connect(&update_timer, SIGNAL(timeout()), this, SLOT(update()));
	//	update_timer.start(1000);
		update();
	}
}

void rss_gui::update_items() {

	//Clear out the current items.
	while (items.size()) {
		v_layout.removeWidget(items.back());
		delete items.back();
		items.pop_back();
	}

	//Add the new ones.
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

void rss_gui::paintEvent(QPaintEvent *e) {

	QDesktopWidget *desktop = QApplication::desktop();
	int width = desktop->width();
	move((width - this->width())-1-offset, 410);

	fragment::paintEvent(e);
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
		//desc_window.offset = width() + 10;
		desc_window.move(x() - desc_window.width() - 10, y());
	}
}

void rss_gui::refresh_update() {

	update_timer.stop();

	//TODO Find otu how to manually invoke QT signals.
	//update_timer.timeout();
	update();

	update_timer.start();
	
}

void rss_gui::update() {

	std::string xml = rss_grabber::grab_xml(url.c_str());
	item_list = rss_parser::parse_xml(xml);

	update_items();

	auto cb = std::bind(&rss_gui::add_path, this, std::placeholders::_1, std::placeholders::_2);
	std::thread t(rss_grabber::process_img_list, item_list, std::move(cb)); t.detach();
}
