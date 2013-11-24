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
#include "fragment.hpp"

class rss_gui : public fragment { 

	public: 
	rss_gui(QWidget *parent = NULL);
	void add_item(const std::string&);

	protected:
	std::vector<QPushButton*> items;
	QVBoxLayout v_layout, base_layout;
	QScrollArea s;
	QWidget contents;

	QFont item_font;
	QPalette item_bg;
		
};
#endif
