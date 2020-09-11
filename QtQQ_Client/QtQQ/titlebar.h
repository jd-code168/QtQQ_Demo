#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>


enum ButtonType {
	MIN_BUTTON = 0,       //��С�����رհ�ť
	MIN_MAX_BUTTON,     //��С������󻯼��رհ�ť
	ONLY_CLOSE_BUTTON   //ֻ�йرհ�ť
};

//�Զ��������
class TitleBar : public QWidget
{
	Q_OBJECT

public:
	TitleBar(QWidget *parent = nullptr);
	~TitleBar();

	void setTitleIcon(const QString &filePath); //���ñ�����ͼ��
	void setTitleContent(const QString &titleContent); //���ñ�����ͼ��
	void setTitleWidth(int width);				 //���ñ���������
	void setButonType(ButtonType buttonType);	 //���ñ�������ť����

	//���洰�����ǰ��λ�ü���С
	void saveRestoreInfo(const QPoint &point, const QSize &size);
	void getRestoreInfo(QPoint &point, QSize &size);

private:
	void paintEvent(QPaintEvent *event);	//��ͼ�¼�
	void mouseDoubleClickEvent(QMouseEvent *event);	 //���˫���¼�
	void mousePressEvent(QMouseEvent *event);	//��갴���¼�
	void mouseMoveEvent(QMouseEvent *event);	//����ƶ��¼�
	void mouseReleaseEvent(QMouseEvent *event);	//����ɿ��¼�

	void initControl();		//��ʼ���ؼ�
	void initConnections(); //��ʼ���ź��������
	void loadStyleSheet(const QString &sheetName);	//������ʽ��

signals:
	// ��������ť���ʱ�������ź�
	void signalButtonMinClicked();	//��С����ť
	void signalButtonRestoreClicked(); //��󻯻�ԭ��ť
	void signalButtonMaxClicked();	//��󻯰�ť
	void signalButtonCloseClicked();	//�رհ�ť

private slots:
	//��ť��Ӧ�Ĳ�
	void onButtonMixClicked();
	void onButtonRestoreClicked();
	void onButtonMaxClicked();
	void onButtonCloseClicked();

private:
	QLabel *m_pIcon;			//������ͼ��
	QLabel *m_pTitleContent;	//����������

	QPushButton *m_pButtonMin;	//��С����ť
	QPushButton *m_pButtonRestore; //��󻯻�ԭ��ť
	QPushButton	*m_pButtonMax;		//��󻯰�ť
	QPushButton *m_pButtonClose;	//�رհ�ť

	//��󻯻�ԭ��ť���������洰���С��λ����Ϣ��
	QPoint m_restorePos;
	QSize m_restoreSize;

	//�ƶ�����ı���
	bool m_isPressed;
	QPoint m_startMovePos;

	QString m_titleContent;		//����������
	ButtonType m_buttonType;	//���������Ͻǰ�ť����

};