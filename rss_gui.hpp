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

class rss_gui : public fragment { 

	Q_OBJECT


	public: 
	rss_gui(QWidget *parent = NULL);
	void add_item(const std::wstring&);

	protected:
	std::vector<QPushButton*> items;
	QVBoxLayout v_layout, base_layout;
	QScrollArea s;
	QWidget contents;

	QFont item_font;
	QPalette item_bg;
	desc_gui desc_window;

	public slots:
	void open_desc();
		
};
#endif
