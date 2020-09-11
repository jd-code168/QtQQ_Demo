#ifndef _BASICWINDOW_H

#include <QFile>
#include <QStyleOption>
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include "basicwindow.h"
#include "commonutils.h"
#include "notifymanager.h"
#include <QSqlQuery>

#endif // !_BASICWINDOW_H

extern QString gLoginEmployeeID;

BasicWindow::BasicWindow(QWidget *parent)
	: QDialog(parent)
{
	m_colorBackGround = CommonUtils::getDefaultSkinColor();
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground, true);
	connect(NotifyManager::getInstance(), SIGNAL(signalSkinChanged(const QColor&)), this, SLOT(onSignalSkinChanged(const QColor&)));
}

BasicWindow::~BasicWindow()
{
}

void BasicWindow::onSignalSkinChanged(const QColor &color) {
	m_colorBackGround = color;
	loadStyleSheet(m_styleName);
}

void BasicWindow::setTitleBarTitle(const QString &title, const QString &icon) {
	_titleBar->setTitleIcon(icon);
	_titleBar->setTitleContent(title);
}

void BasicWindow::initTitleBar(ButtonType buttontype) {
	_titleBar = new TitleBar(this);
	_titleBar->setButonType(buttontype);
	_titleBar->move(0, 0);

	connect(_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
	connect(_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
	connect(_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
	connect(_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));
}

void BasicWindow::loadStyleSheet(const QString &sheetName) {
	m_styleName = sheetName;
	QFile file(":/Resources/QSS/" + sheetName + ".css");
	file.open(QFile::ReadOnly);
	
	if (file.isOpen()){
		this->setStyleSheet("");
		QString qsstyleSheet = QLatin1String(file.readAll());
		
		//获取用户当前的皮肤RGB值,用以设置背景颜色
		QString r = QString::number(m_colorBackGround.red());
		QString g= QString::number(m_colorBackGround.green());
		QString b = QString::number(m_colorBackGround.blue());

		qsstyleSheet += QString("QWidget[titleskin=true]\
							 {background-color:rgb(%1,%2,%3);\
							  border-top-left-radius:4px;}\
							  QWidget[bottomskin=true]\
							  {border-top:1px solid rgba(%1,%2,%3,100);\
							  background-color:rgba(%1,%2,%3,50);\
							  border-bottom-left-radius:4px;\
							  border-bottom-right-radius:4px;}").arg(r).arg(g).arg(b);
		this->setStyleSheet(qsstyleSheet);
		
	}

	file.close();
}

//背景颜色
void BasicWindow::initBackGroundColor() {
	QStyleOption opt;
	opt.init(this);

	QPainter p(this);
	this->style()->drawPrimitive(QStyle::PE_Widget, &opt,&p,this);
	
}

//子类化部件时，需要重写绘图事件设置背景图
void BasicWindow::paintEvent(QPaintEvent *event) {
	initBackGroundColor();
	QDialog::paintEvent(event);
}

/*
==============================头像转圆头像====================================
参数
const QPixmap &src ：图片来源
QPixmap &mask ：圆形背景
QSize maskSize ：圆头像大小
*/
QPixmap BasicWindow::getRoundImage(const QPixmap &src,QPixmap &mask,QSize maskSize) {
	if (QSize(0, 0) == maskSize) {
		maskSize = mask.size();
	}
	else {
		mask=mask.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}

	//保存转换后的图像
	QImage resultImage(maskSize, QImage::Format_ARGB32_Premultiplied);
	QPainter painter(&resultImage);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(resultImage.rect(),Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawPixmap(0, 0, mask);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.drawPixmap(0, 0, src.scaled(maskSize,Qt::KeepAspectRatio, Qt::SmoothTransformation));
	painter.end();

	return QPixmap::fromImage(resultImage);
}

void BasicWindow::onShowClose(bool) {
	this->close();
}

void BasicWindow::onShowHide(bool) {
	this->hide();
}

void BasicWindow::onShowMin(bool) {
	this->showMinimized();
}

void BasicWindow::onShowNormal(bool) {
	this->show();
	this->activateWindow();
}

void BasicWindow::onShowQuit(bool) {

	//更新登陆状态为离线
	QString strSqlStatus = QString("UPDATE tab_employees SET online=1 WHERE employeeID=%1").arg(gLoginEmployeeID.toInt());
	QSqlQuery sqlStatus(strSqlStatus);
	sqlStatus.exec();

	QApplication::quit();
}

//鼠标移动事件
void BasicWindow::mouseMoveEvent(QMouseEvent *e) {
	if (m_mousePressed && (e->buttons() & Qt::LeftButton)) {
		///*m_mousePoint = event->globalPos();
		move(e->globalPos() -m_mousePoint);
		e->accept();
	}
}

//鼠标按下事件
void BasicWindow::mousePressEvent(QMouseEvent *e) {
	if (e->buttons() == Qt::LeftButton) {
		m_mousePressed = true;
		m_mousePoint = e->globalPos()-pos();
		e->accept();
	}
}

//鼠标释放事件
void BasicWindow::mouseReleaseEvent(QMouseEvent *event) {
	m_mousePressed = false;
}

//
void BasicWindow::onButtonMinClicked() {
	if (Qt::Tool == (this->windowFlags() & Qt::Tool)) {
		this->hide();
	}
	else {
		this->showMinimized();
	}
}

void BasicWindow::onButtonRestoreClicked() {
	QPoint windowPos;
	QSize windowSize;
	_titleBar->getRestoreInfo(windowPos, windowSize);
	this->setGeometry(QRect(windowPos, windowSize));	//设置窗口的几何属性
}

void BasicWindow::onButtonMaxClicked() {
	_titleBar->saveRestoreInfo(this->pos(), QSize(this->width(), this->height()));

	//通过desktop()->availableGeometry()返回当前的桌面几何属性
	QRect desktopRect = QApplication::desktop()->availableGeometry();
	QRect factRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
	this->setGeometry(factRect);
}

void BasicWindow::onButtonCloseClicked() {
	this->close();
}

