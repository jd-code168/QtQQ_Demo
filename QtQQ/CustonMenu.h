#pragma once

#include <QMenu>

class CustonMenu : public QMenu
{
	Q_OBJECT

public:
	CustonMenu(QWidget *parent=nullptr);
	~CustonMenu();

public:
	void addCustomMenu(const QString& text, const QString& icon,const QString &name );
	QAction* getAction(const QString &text);
private:
	QMap<QString, QAction*>m_menuActionMap;
};
