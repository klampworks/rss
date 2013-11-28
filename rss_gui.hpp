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
#include "desc_gui.hpp"
#include "rss_item.hpp"
#include <QSignalMapper>

class rss_gui : public fragment { 

	Q_OBJECT

	public: 
	rss_gui(QWidget *parent = NULL);
	void add_item(rss_item);

	protected:
	std::vector<QPushButton*> items;
	std::vector<rss_item> item_list;
	QVBoxLayout v_layout, base_layout;
	QScrollArea s;
	QWidget contents;

	QFont item_font;
	QPalette item_bg;
	desc_gui desc_window;

	QSignalMapper sigmap;
	public slots:
	void open_desc(int);
		
};
#endif
