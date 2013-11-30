#include "desc_gui.hpp"
#include <QDesktopWidget>
#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPalette>
#include <QScrollArea>
#include <QScrollBar>

desc_gui::desc_gui(QColor bg_colour, QWidget *parent) : 
	fragment(bg_colour, parent),
	description(),
	layout(this) {

	setMaximumHeight(200);
	setFixedHeight(200);

	//QTextEdit containing the description text.
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
