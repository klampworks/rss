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
	desc_gui(QColor, QWidget *parent = NULL);
	void add_desc(QString &&);
	void set_image(const std::string &);

	protected:
	QTextEdit description;
	QHBoxLayout layout;
	QLabel image;

		
};
#endif
