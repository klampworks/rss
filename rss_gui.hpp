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
#include <QTimer>

class rss_gui : public fragment { 

	Q_OBJECT

	public: 
	rss_gui(std::string, QColor, QWidget *parent = NULL);
	void add_path(unsigned, std::string&&);
	void paintEvent(QPaintEvent *e);

	protected:
	std::vector<QPushButton*> items;
	std::map<unsigned, rss_item> item_list;
	QVBoxLayout v_layout, base_layout;
	QScrollArea s;
	QWidget contents;

	QHBoxLayout topbar;
	QLabel topbar_title;
	QPushButton settings;
	QPushButton refresh;

	QFont item_font;
	QPalette item_bg;
	desc_gui desc_window;

	QTimer update_timer;
	QSignalMapper sigmap;

	std::string url;
	void update_items();

	public slots:
	void open_desc(int);
	void update();
	void refresh_update();
		
};
#endif
