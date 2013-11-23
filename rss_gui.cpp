#include "rss_gui.hpp"
#include <QDesktopWidget>
#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPalette>
#include <QScrollArea>


rss_gui::rss_gui(QWidget *parent) : QWidget(parent) {

	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_X11DoNotAcceptFocus);
	
	this->corner = 15;
	this->colour = QColor(11, 11, 44, 127);
	setMaximumHeight(200);
	setFixedHeight(200);

/*
	QVBoxLayout *t = new QVBoxLayout(this);
	QScrollArea *s = new QScrollArea(this);

	QWidget *contents = new QWidget(s);
	v_layout = new QVBoxLayout(contents);
	contents->setLayout(v_layout);

	this->setLayout(t);
	t->addWidget(s);
	contents->show();
	s->setWidget(contents);
*/

	//Add a layout to the main window.
	QVBoxLayout *t = new QVBoxLayout(this);
	this->setLayout(t);

	//Create a scrollarea, add it to the main window layout
	QScrollArea *s = new QScrollArea(this);
	s->setWidgetResizable(true); //this is fucking important.

	QPalette pal;
	pal.setColor(QPalette::Button,Qt::transparent);
	s->setPalette(pal);
	s->setAttribute(Qt::WA_TranslucentBackground);
	s->setStyleSheet("background-color:transparent;border:0px;");



	t->addWidget(s);

	//Create a plane for the scrollable contents.
	QWidget *contents = new QWidget(s);

	//Create a layout for the conents.
	v_layout = new QVBoxLayout(contents);
	contents->setLayout(v_layout);


	//this->setLayout(v_layout);

	s->setWidget(contents);
}

void rss_gui::add_item(const std::string &title) {

	
	QPushButton *b = new QPushButton(QString::fromStdString(title));
	b->setFlat(true);
	//b->setMinimumHeight(10);
	QPalette pal;
	pal.setColor(QPalette::Button,Qt::transparent);
	b->setPalette(pal);
	v_layout->addWidget(b);
	b->show();

	items.push_back(b);
}

void rss_gui::paintEvent(QPaintEvent *e) {

	Q_UNUSED(e);
	QPainter qp(this);
	drawLines(&qp);
	QDesktopWidget *desktop = QApplication::desktop();
	int width = desktop->width();
	move((width - this->width())-1, 410);
}

void rss_gui::drawLines(QPainter *qp) {

	qp->setOpacity(1.0);
	QPen pen(Qt::NoPen);
	qp->setPen(pen);

 	QBrush brush;
	brush.setStyle(Qt::SolidPattern); 
      	brush.setColor(this->colour);
	qp->setBrush(brush);

	qp->drawRoundedRect(0,0, this->width(), this->height(), this->corner, this->corner);
}
