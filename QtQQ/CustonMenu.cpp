#include "CustonMenu.h"
#include "commonutils.h"

CustonMenu::CustonMenu(QWidget *parent)
	: QMenu(parent)
{
	setAttribute(Qt::WA_TranslucentBackground);
	CommonUtils::loadStyleSheet(this, "Menu");
}

CustonMenu::~CustonMenu()
{
}

void CustonMenu::addCustomMenu(const QString & text, const QString & icon, const QString & name)
{
	QAction	*pAction = addAction(QIcon(icon), name);
	m_menuActionMap.insert(text, pAction);
}

QAction * CustonMenu::getAction(const QString & text)
{
	return m_menuActionMap[text];
}
