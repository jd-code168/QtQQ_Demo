#include "CCMainWindow.h"
#include <QProxystyle>
#include <QPainter>
#include "SkinWindow.h"
#include "SysTray.h"
#include <QEvent>
#include "notifymanager.h"
#include "rootcontatitem.h"
#include "contactitem.h"
#include "talkwindowshell.h"
#include <QMouseEvent>
#include <QApplication>
#include "windowmanager.h"
#include <QTreeWidgetItem>
#include <QHBoxLayout>
#include <QSqlQuery>

QString gstrLoginHeadPath;
extern QString gLoginEmployeeID;

class CustomProxyStyle :public QProxyStyle {
public:
	virtual void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
		QPainter *painter, const QWidget *widget = nullptr) const {
		if (PE_FrameFocusRect == element) {
			return;
		}
		else {
			QProxyStyle::drawPrimitive(element, option, painter, widget);
		}

	}
};

CCMainWindow::CCMainWindow(QString account, bool isAccountLogin, QWidget *parent)
	: BasicWindow(parent)
	,m_isAccountLogin(isAccountLogin)
	,m_account(account)
{
	ui.setupUi(this);
	setWindowFlags(windowFlags() | Qt::Tool);
	loadStyleSheet("CCMainWindow");

	setHeadPixmap(getHeadPicturePath());
	initControl();
	initTimer();
}

CCMainWindow::~CCMainWindow()
{
}

void CCMainWindow::initTimer()
{
	QTimer *timer = new QTimer(this);
	timer->setInterval(500);
	connect(timer, &QTimer::timeout, [this]{
		static int level = 0;
		if (level == 99) {
			level = 0;
		}
		level++;
		setLevelPixmap(level);
	});
	timer->start();
}

void CCMainWindow::initControl()
{
	//����ȡ����ʱ�����Ʊ߿�
	ui.treeWidget->setStyle(new CustomProxyStyle);
	setLevelPixmap(0);
	//setHeadPixmap(":/Resources/MainWindow/girl.png");
	setStatusMenuIcon(":/Resources/MainWindow/StatusSucceeded.png");

	QHBoxLayout *appupLayout = new QHBoxLayout;
	appupLayout->setContentsMargins(0, 0, 0, 0);
	appupLayout->addWidget(addOterAppExtension(":/Resources/MainWindow/app/app_7.png", "app_7"));
	appupLayout->addWidget(addOterAppExtension(":/Resources/MainWindow/app/app_2.png", "app_2"));
	appupLayout->addWidget(addOterAppExtension(":/Resources/MainWindow/app/app_3.png", "app_3"));
	appupLayout->addWidget(addOterAppExtension(":/Resources/MainWindow/app/app_4.png", "app_4"));
	appupLayout->addWidget(addOterAppExtension(":/Resources/MainWindow/app/app_5.png", "app_5"));
	appupLayout->addWidget(addOterAppExtension(":/Resources/MainWindow/app/app_6.png", "app_6"));
	appupLayout->addWidget(addOterAppExtension(":/Resources/MainWindow/app/skin.png", "app_skin"));
	appupLayout->setSpacing(2);
	appupLayout->addStretch();
	ui.appWidget->setLayout(appupLayout);

	ui.bottomlayout_up->addWidget(addOterAppExtension(":/Resources/MainWindow/app/app_10.png", "app_10"));
	ui.bottomlayout_up->addWidget(addOterAppExtension(":/Resources/MainWindow/app/app_8.png", "app_8"));
	ui.bottomlayout_up->addWidget(addOterAppExtension(":/Resources/MainWindow/app/app_11.png", "app_11"));
	ui.bottomlayout_up->addWidget(addOterAppExtension(":/Resources/MainWindow/app/app_9.png", "app_9"));
	ui.bottomlayout_up->addStretch();

	initContactTree();

	//����ǩ��
	ui.lineEdit->installEventFilter(this);
	//��������
	ui.searchLineEdit->installEventFilter(this);

	connect(ui.sysmin, SIGNAL(clicked(bool)), this, SLOT(onShowHide(bool)));
	connect(ui.sysclose, SIGNAL(clicked(bool)), this, SLOT(onShowClose(bool)));
	
	connect(NotifyManager::getInstance(), &NotifyManager::signalSkinChanged, [this] {updateSeachStyle(); });

	SysTray* systray = new SysTray(this);
}

QString CCMainWindow::getHeadPicturePath()
{
	QString strPicturePath;

	if (!m_isAccountLogin) { //QQ�ŵ�½
		QSqlQuery queryPicture(QString("SELECT picture FROM tab_employees WHERE employeeID=%1").arg(gLoginEmployeeID));
		queryPicture.exec();
		queryPicture.finish();

		strPicturePath = queryPicture.value(0).toString();
	}
	else { //�˺ŵ�½
		QSqlQuery queryEmployeeID(QString("SELECT employeeID FROM tab_accounts WHERE account='%1'").arg(m_account));
		queryEmployeeID.exec();
		queryEmployeeID.first();
		int employeeID = queryEmployeeID.value(0).toInt();

		QSqlQuery queryPicture(QString("SELECT picture FROM tab_employees WHERE employeeID=%1").arg(employeeID));
		queryPicture.exec();
		queryPicture.first();

		strPicturePath = queryPicture.value(0).toString();
	}

	gstrLoginHeadPath = strPicturePath;
	return strPicturePath;
}

void CCMainWindow::updateSeachStyle()
{
	ui.searchWidget->setStyleSheet(QString(
		"QWidget#searchWidget{background-color:rgba(%1,%2,%3,50);"
		"border-bottom:1px solid rgba(%1,%2,%3��30)}"
		"QPushButton#searchBtn{border-image:url(:/Resources/MainWindow/search/search_icon.png)}")
		.arg(m_colorBackGround.red())
		.arg(m_colorBackGround.green())
		.arg(m_colorBackGround.blue()));
}

void CCMainWindow::addCompanyDeps(QTreeWidgetItem * pRootGroupItem, int DepID)
{
	QTreeWidgetItem* pChild = new QTreeWidgetItem;

	QPixmap pix;
	pix.load(":/Resources/MainWindow/head_mask.png");
	//����ӽڵ�,ֵΪ1������������
	pChild->setData(0, Qt::UserRole,1); //����������Ϊ1
	pChild->setData(0, Qt::UserRole + 1, DepID);

	//��ȡ��˾������ͷ��
	QPixmap groupPix;
	QSqlQuery queryPicture(QString("SELECT picture FROM tab_department WHERE departmentID=%1").arg(DepID));
	queryPicture.exec();
	queryPicture.next();
	groupPix.load(queryPicture.value(0).toString());

	//��ȡ��������
	QString strDepName;
	QSqlQuery querDepName(QString("SELECT department_name FROM tab_department WHERE departmentID=%1").arg(DepID));
	querDepName.exec();
	querDepName.first();
	strDepName = querDepName.value(0).toString();

	ContactItem* pContactItem = new ContactItem(ui.treeWidget);
	pContactItem->SetHeadPixmap(getRoundImage(groupPix, pix, pContactItem->getHeadLabelSize()));
	pContactItem->setUserName(strDepName);
	
	pRootGroupItem->addChild(pChild);
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);

	//m_groupMap.insert(pChild, sDeps);
}

void CCMainWindow::setUserName(const QString & username)
{
	ui.nameLabel->adjustSize();
	
	//�ı����������ʡ��...
	//fontMetrics()����QFontMetrics�����
	QString name=ui.nameLabel->fontMetrics().elidedText(username, Qt::ElideRight, ui.nameLabel->width());
	ui.nameLabel->setText(name);
}

void CCMainWindow::setLevelPixmap(int level)
{
	QPixmap levelPixmap(ui.levelBtn->size());
	levelPixmap.fill(Qt::transparent);

	QPainter painter(&levelPixmap);
	painter.drawPixmap(0, 4, QPixmap(":/Resources/MainWindow/lv.png"));

	int unitNum = level % 10; //��λ��
	int tenNum = level / 10; //ʮλ��

	//ʮλ,��ȡͼƬ�еĲ��ֽ��л���
	//drawPixmap(���Ƶ�x,���Ƶ�y,ͼƬ��ͼƬ���Ͻ�x,ͼƬ���Ͻ�y,�����Ŀ�ȣ������ĸ߶�)
	painter.drawPixmap(10, 4, QPixmap(":/Resources/MainWindow/levelvalue.png"), tenNum * 6, 0, 6, 7);
	//��λ
	painter.drawPixmap(16, 4, QPixmap(":/Resources/MainWindow/levelvalue.png"), unitNum * 6, 0, 6, 7);
	ui.levelBtn->setIcon(levelPixmap);
	ui.levelBtn->setIconSize(ui.levelBtn->size());
}

void CCMainWindow::setHeadPixmap(const QString headPath)
{
	QPixmap pix;
	pix.load(":/Resources/MainWindow/head_mask.png");
	ui.headLabel->setPixmap(getRoundImage(QPixmap(headPath), pix, ui.headLabel->size()));
}

void CCMainWindow::setStatusMenuIcon(const QString & statusPath)
{
	QPixmap statusBtnPixmap(ui.stausBtn->size());
	statusBtnPixmap.fill(Qt::transparent);

	QPainter painter(&statusBtnPixmap);
	painter.drawPixmap(4, 4, QPixmap(statusPath));

	ui.stausBtn->setIcon(statusBtnPixmap);
	ui.stausBtn->setIconSize(ui.stausBtn->size());
}

QWidget * CCMainWindow::addOterAppExtension(const QString & apppath, const QString & appName)
{
	QPushButton* btn = new QPushButton(this);
	btn->setFixedSize(20, 20);

	QPixmap pixmap(btn->size());
	pixmap.fill(Qt::transparent);

	QPainter painter;//(&pixmap);
	QPixmap appPixmap(apppath);
	painter.begin(&pixmap);
	painter.drawPixmap((btn->width() - appPixmap.width()) / 2,
			(btn->height()-appPixmap.height())/2,appPixmap);
	painter.end();
	btn->setIcon(pixmap);
	btn->setIconSize(btn->size());
	btn->setObjectName(appName);
	btn->setProperty("hasborder", true);

	connect(btn, &QPushButton::clicked, this, &CCMainWindow::onAppIconClicked);
	return btn;
}

void CCMainWindow::initContactTree()
{
	//չ��������ʱ���ź�
	connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(onItemClicked(QTreeWidgetItem*, int)));
	connect(ui.treeWidget, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(onItemExpanded(QTreeWidgetItem*)));
	connect(ui.treeWidget, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(onItemCollapsed(QTreeWidgetItem*)));
	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(onItemDoubleClicked(QTreeWidgetItem*, int)));
	
	//���ڵ�
	QTreeWidgetItem* pRootGroupItem = new QTreeWidgetItem;
	pRootGroupItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	pRootGroupItem->setData(0, Qt::UserRole, 0); //����������Ϊ0
	RootContatItem *pItemName = new RootContatItem(true, ui.treeWidget);
	
	//��ȡ��˾����ID
	QSqlQuery queryCompDepID(QString("SELECT departmentID FROM tab_department WHERE department_name='%1'").arg(QString::fromLocal8Bit("��˾Ⱥ")));
	queryCompDepID.exec();
	queryCompDepID.first();
	int CompDepID = queryCompDepID.value(0).toInt();

	//��ȡQQ��½¼�����ڲ���ID
	QSqlQuery querySelfDepID(QString("SELECT departmentID FROM tab_employees WHERE employeeID=%1").arg(gLoginEmployeeID.toInt()));
	querySelfDepID.exec();
	querySelfDepID.first();
	int SelfDepID = querySelfDepID.value(0).toInt();
	
	//��ʼ����˾Ⱥ����½�����ڵ�Ⱥ
	addCompanyDeps(pRootGroupItem, CompDepID);
	addCompanyDeps(pRootGroupItem, SelfDepID);

	QString strGroupName = QString::fromLocal8Bit("Demo");
	pItemName->setText(strGroupName);

	//�������ڵ�
	ui.treeWidget->addTopLevelItem(pRootGroupItem);
	ui.treeWidget->setItemWidget(pRootGroupItem, 0, pItemName);

//	QStringList sCompDeps;  //��˾����
//	sCompDeps << QString::fromLocal8Bit("��˾Ⱥ");
//	sCompDeps << QString::fromLocal8Bit("���²�");
//	sCompDeps << QString::fromLocal8Bit("�з���");
//	sCompDeps << QString::fromLocal8Bit("�г���");
//
//	sCompDeps.append(QString::fromLocal8Bit("�ۺϲ�"));
//
//	for (int nIndex = 0; nIndex < sCompDeps.length(); nIndex++) {
//		addCompanyDeps(pRootGroupItem, sCompDeps.at(nIndex));
//	}
}

void CCMainWindow::resizeEvent(QResizeEvent * event)
{
	setUserName(QString::fromLocal8Bit("Qt-Demo$$$$$$$$"));
	BasicWindow::resizeEvent(event);
}

bool CCMainWindow::eventFilter(QObject * obj, QEvent * event)
{
	if (ui.searchLineEdit == obj) {
		//���̽����¼�
		if (event->type() == QEvent::FocusIn) {
			ui.searchWidget->setStyleSheet(QString(
										"QWidget#searchWidget{background-color:rgb(255, 255, 255);"
										"border-bottom:1px solid rgba(%1,%2,%3��100)}"
										"QPushButton#searchBtn{border-image:url(:/Resources/MainWindow/search/main_search_deldown.png)}"
										"QPushButton#searchBtn:hover{border-image:url(:/Resources/MainWindow/search/main_search_delhighlight.png)}"
										"QPushButton#searchBtn:pressed{border-image:url(:/Resources/MainWindow/search/main_search_delhightdown.png)}")
										.arg(m_colorBackGround.red())
										.arg(m_colorBackGround.green())
										.arg(m_colorBackGround.blue()));
		}
		else if (event->type() == QEvent::FocusOut) {
			updateSeachStyle();
		}
		
	}
	return false;
}

void CCMainWindow::mousePreeEvent(QMouseEvent * event)
{
	if (qApp->widgetAt(event->pos())!=ui.searchLineEdit && ui.searchLineEdit->hasFocus()){

		ui.searchLineEdit->clearFocus();
	}
	else if (qApp->widgetAt(event->pos()) != ui.lineEdit && ui.lineEdit->hasFocus()) {

		ui.lineEdit->clearFocus();
	}
	BasicWindow::mousePressEvent(event);
}

void CCMainWindow::onItemClicked(QTreeWidgetItem * item, int column)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (!bIsChild) {
		item->setExpanded(!item->isExpanded()); //δչ����չ������
	}
}

void CCMainWindow::onItemExpanded(QTreeWidgetItem * item)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (!bIsChild) {
		//dynamic_cast ���������ָ�루�����ã�ת�����̳���ָ��
		RootContatItem* prootItem = dynamic_cast<RootContatItem *>(ui.treeWidget->itemWidget(item, 0));
		
		if (prootItem) {
			prootItem->setExpanded(true);
		}
		
	}
	
}

void CCMainWindow::onItemCollapsed(QTreeWidgetItem * item)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (!bIsChild) {
		//dynamic_cast ���������ָ�루�����ã�ת�����̳���ָ��
		RootContatItem* prootItem = dynamic_cast<RootContatItem *>(ui.treeWidget->itemWidget(item, 0));
		if (prootItem) {
			prootItem->setExpanded(false);
		}

	}
}

void CCMainWindow::onItemDoubleClicked(QTreeWidgetItem * item, int column)
{
	bool bIsChild = item->data(0,Qt::UserRole).toBool();
	if (bIsChild) {

		WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString());

		//QString strGroup = m_groupMap.value(item);

	/*	if (strGroup == QString::fromLocal8Bit("��˾Ⱥ")) {
			
			WindowManager::getInstance()->addNewTalkWindow(item->data(0,Qt::UserRole+1).toString(), COMPANY);

		}else if (strGroup == QString::fromLocal8Bit("���²�")) {
			WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString(), PERSONELGROUP);
		}
		else if (strGroup == QString::fromLocal8Bit("�г���")) {
			WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString(), MARKETGROUP);
		}
		else if (strGroup == QString::fromLocal8Bit("�з���")) {
			WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString(), DEVELOPMENTGROUP);
		}*/
	}
}

void CCMainWindow::onAppIconClicked()
{
	if (sender()->objectName ()== "app_skin") {
		SkinWindow* skinWindow = new SkinWindow;
		skinWindow->show();
	}
}

