#include "rss_gui.hpp"
#include <QDesktopWidget>
#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPalette>
#include <QScrollArea>
#include <QScrollBar>


rss_gui::rss_gui(QWidget *parent) : fragment(parent), desc_window(), sigmap(this) {

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
}

void rss_gui::add_item(rss_item item) {

	QPushButton *b = new QPushButton(QString::fromWCharArray(item.title.c_str()));
	b->setFlat(true);
	b->setFont(item_font);
	b->setPalette(item_bg);
	//connect(b, SIGNAL(clicked()), this, SLOT(open_desc(item_list.size())));
	QObject::connect(b, SIGNAL(clicked()), &sigmap, SLOT(map()));
	sigmap.setMapping(b, item_list.size());

	v_layout.addWidget(b);
	b->show();

	items.push_back(b);
	item_list.push_back(item);
}

#include <iostream>
void rss_gui::open_desc(int i) {

	if (desc_window.isVisible()) {
		desc_window.hide();
	} else {
		desc_window.show();
	desc_window.offset = width() + 10;
	}

	desc_window.add_desc(QString::fromWCharArray(item_list[i].description.c_str()));

}
