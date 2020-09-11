#include "contactitem.h"

ContactItem::ContactItem(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initControl();
}

ContactItem::~ContactItem()
{
}

void ContactItem::setUserName(const QString & userName)
{
	ui.userName->setText(userName);
}

void ContactItem::setSignName(const QString & signName)
{
	ui.signName->setText(signName);
}

void ContactItem::SetHeadPixmap(const QPixmap & headPath)
{
	ui.label->setPixmap(headPath);
}

QString ContactItem::getUserName() const
{
	return QString(ui.userName->text());
}

QSize ContactItem::getHeadLabelSize() const
{
	return ui.label->size();
}

void ContactItem::initControl()
{
}
