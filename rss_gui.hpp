#ifndef RSS_GUI_HPP
#define RSS_GUI_HPP

#include <QWidget>
#include <QPainter>
#include <QPainter>
#include <QColor>
#include <QBoxLayout>
#include <vector>
#include <QPushButton>
#include <string>
#include <QScrollArea>

class rss_gui : public QWidget { 

	public: 
	rss_gui(QWidget *parent = NULL);
	void paintEvent(QPaintEvent *e);
	void drawLines(QPainter *qp);
	void add_item(const std::string&);

	protected:
	QColor colour;
	unsigned corner;
	std::vector<QPushButton*> items;
	QVBoxLayout *v_layout;
	QScrollArea *s;
		
};
#endif
