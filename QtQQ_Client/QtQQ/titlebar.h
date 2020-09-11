#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>


enum ButtonType {
	MIN_BUTTON = 0,       //最小化及关闭按钮
	MIN_MAX_BUTTON,     //最小化、最大化及关闭按钮
	ONLY_CLOSE_BUTTON   //只有关闭按钮
};

//自定义标题栏
class TitleBar : public QWidget
{
	Q_OBJECT

public:
	TitleBar(QWidget *parent = nullptr);
	~TitleBar();

	void setTitleIcon(const QString &filePath); //设置标题栏图标
	void setTitleContent(const QString &titleContent); //设置标题栏图标
	void setTitleWidth(int width);				 //设置标题栏长度
	void setButonType(ButtonType buttonType);	 //设置标题栏按钮类型

	//保存窗口最大化前的位置及大小
	void saveRestoreInfo(const QPoint &point, const QSize &size);
	void getRestoreInfo(QPoint &point, QSize &size);

private:
	void paintEvent(QPaintEvent *event);	//绘图事件
	void mouseDoubleClickEvent(QMouseEvent *event);	 //鼠标双击事件
	void mousePressEvent(QMouseEvent *event);	//鼠标按下事件
	void mouseMoveEvent(QMouseEvent *event);	//鼠标移动事件
	void mouseReleaseEvent(QMouseEvent *event);	//鼠标松开事件

	void initControl();		//初始化控件
	void initConnections(); //初始化信号与槽连接
	void loadStyleSheet(const QString &sheetName);	//加载样式表

signals:
	// 标题栏按钮点击时触发的信号
	void signalButtonMinClicked();	//最小化按钮
	void signalButtonRestoreClicked(); //最大化还原按钮
	void signalButtonMaxClicked();	//最大化按钮
	void signalButtonCloseClicked();	//关闭按钮

private slots:
	//按钮相应的槽
	void onButtonMixClicked();
	void onButtonRestoreClicked();
	void onButtonMaxClicked();
	void onButtonCloseClicked();

private:
	QLabel *m_pIcon;			//标题栏图标
	QLabel *m_pTitleContent;	//标题栏内容

	QPushButton *m_pButtonMin;	//最小化按钮
	QPushButton *m_pButtonRestore; //最大化还原按钮
	QPushButton	*m_pButtonMax;		//最大化按钮
	QPushButton *m_pButtonClose;	//关闭按钮

	//最大化还原按钮变量（保存窗体大小和位置信息）
	QPoint m_restorePos;
	QSize m_restoreSize;

	//移动窗体的变量
	bool m_isPressed;
	QPoint m_startMovePos;

	QString m_titleContent;		//标题栏内容
	ButtonType m_buttonType;	//标题栏右上角按钮类型

};