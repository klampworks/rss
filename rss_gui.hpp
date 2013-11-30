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
#include <map>

class rss_gui : public fragment { 

	Q_OBJECT

	public: 
	rss_gui(QWidget *parent = NULL);
	void add_items(std::map<unsigned, rss_item>);
	void add_path(unsigned, std::string&&);

	protected:
	std::vector<QPushButton*> items;
	std::map<unsigned, rss_item> item_list;
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
