#ifndef FRAGMENT_HPP
#define FRAGMENT_HPP

#include <QWidget>
#include <QPainter>
#include <QPainter>
#include <QColor>
#include <QBoxLayout>
#include <vector>
#include <QPushButton>
#include <string>
#include <QScrollArea>

class fragment : public QWidget { 

	public: 
	fragment(QWidget *parent = NULL);
	void paintEvent(QPaintEvent *e);
	void draw_lines(QPainter *qp);

	protected:
	QColor colour;
	unsigned corner;
		
};
#endif
