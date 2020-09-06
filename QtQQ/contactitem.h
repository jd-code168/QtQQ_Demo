#pragma once

#include <QWidget>
#include "ui_contactitem.h"

class ContactItem : public QWidget
{
	Q_OBJECT

public:
	ContactItem(QWidget *parent = Q_NULLPTR);
	~ContactItem();

	void setUserName(const QString& userName);
	void setSignName(const QString& signName);
	void SetHeadPixmap(const QPixmap& headPath);
	QString getUserName()const;
	QSize getHeadLabelSize()const;

private:
	void initControl();

private:
	Ui::ContactItem ui;
};
