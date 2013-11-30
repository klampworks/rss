#ifndef DESC_GUI_HPP
#define DESC_GUI_HPP

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
#include <QTextEdit>
#include <QLabel>

class desc_gui : public fragment { 

	public: 
	desc_gui(QWidget *parent = NULL);
	void add_desc(QString &&);
	void set_image(const std::string &);

	protected:
	std::vector<QPushButton*> items;
	QVBoxLayout v_layout, base_layout;
	QScrollArea s;
	QWidget contents;

	QFont item_font;
	QPalette item_bg;

	QTextEdit description;
	QHBoxLayout layout;
	QLabel image;

		
};
#endif
