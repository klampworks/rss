#include "desc_gui.hpp"
#include <QDesktopWidget>
#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPalette>
#include <QScrollArea>
#include <QScrollBar>

desc_gui::desc_gui(QWidget *parent) : fragment(parent),
	description(),
	layout(this) {

	//TODO: How much of this is inherited from fragment...?
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

	description.setReadOnly(true);
	description.viewport()->setAutoFillBackground(false);
	description.setFrameShape(QFrame::NoFrame);


	//Add a label for the image and make it sqaure
	//with a width and height 90% of the window's height.
	int image_size = height() * 0.9;
	image.setFixedSize(image_size, image_size);

	layout.addWidget(&image);
	layout.addWidget(&description);
	setLayout(&layout);
}

void desc_gui::add_desc(QString && desc) {
	description.setText(desc);
}

void desc_gui::set_image(const std::string &path) {
	image.setPixmap(QPixmap(QString::fromStdString(path)));

}
