#pragma once

#include <QWidget>
#include "ui_qtwidgetsclass.h"

class QtWidgetsClass : public QWidget
{
	Q_OBJECT

public:
	QtWidgetsClass(QWidget *parent = Q_NULLPTR);
	~QtWidgetsClass();

private:
	Ui::QtWidgetsClass ui;
};
