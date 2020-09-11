#include "SysTray.h"
#include <QWidget>
#include "CustonMenu.h"

SysTray::SysTray(QWidget *parent)
	:m_parent(parent)
	,QSystemTrayIcon(parent)
{
	initSystemTray();
	this->show();
}

SysTray::~SysTray()
{
}

void SysTray::initSystemTray()
{
	setToolTip(QStringLiteral("QQ-Demo"));
	setIcon(QIcon(":/Resources/MainWindow/app/logo.ico"));
	connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onIconActivated(QSystemTrayIcon::ActivationReason)));
}

void SysTray::onIconActivated(QSystemTrayIcon::ActivationReason reason) {
	if (reason == QSystemTrayIcon::Trigger) {
		m_parent->show();
		m_parent->activateWindow();
	}
	else if(reason==QSystemTrayIcon::Context)
	{
		addSystrayMenu();
	}
}

void SysTray::addSystrayMenu() {
	CustonMenu *customMenu = new CustonMenu(m_parent);
	customMenu->addCustomMenu("onShow", 
							":/Resources/MainWindow/app/logo.ico",
							QStringLiteral("ÏÔÊ¾"));

	
	customMenu->addCustomMenu("onQuit", 
							":/Resources/MainWindow/app/page_close_btn_hover.png",
							QStringLiteral("ÍË³ö"));

	connect(customMenu->getAction("onShow"), SIGNAL(triggered(bool)),
		m_parent, SLOT(onShowNormal(bool)));
	connect(customMenu->getAction("onQuit"), SIGNAL(triggered(bool)),
		m_parent, SLOT(onShowQuit(bool)));

	customMenu->exec(QCursor::pos());
	delete customMenu;
	customMenu = nullptr;
}