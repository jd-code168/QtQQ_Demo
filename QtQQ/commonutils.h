#pragma once
#include <QProxyStyle>
#include <QPixmap>
#include <QSize>

//�ı�Ĭ�ϵĲ������
class CustomProxyStyle:public QProxyStyle
{
public:
	CustomProxyStyle(QObject *parent)
	{
		this->setParent(parent);
	}
	virtual void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
		QPainter *painter, const QWidget *widget = 0) const {
		if (PE_FrameFocusRect == element)  //PE_FrameFocusRect:�߿��ȡ����
		{
			//ȥ��windows�в���Ĭ�ϵı߿�����߿򣬲�����ȡ����ʱֱ�ӷ��أ������л���
			return;
		}
		else {
			QProxyStyle::drawPrimitive(element, option, painter, widget);
		}
	}
};

class CommonUtils
{
public:
	CommonUtils();
	virtual ~CommonUtils();

public:
	static QPixmap getRoundImage(const QPixmap &src, QPixmap &mask, QSize &maskSize=QSize(0,0));
	static void loadStyleSheet(QWidget *widget, const QString &sheetName);
	static void setDefaultSkinColor(const QColor &color);
	static QColor getDefaultSkinColor();
};

