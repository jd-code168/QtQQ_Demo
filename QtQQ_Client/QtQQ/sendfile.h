#pragma once

#include "basicwindow.h"
#include "ui_sendfile.h"

class SendFile : public BasicWindow
{
	Q_OBJECT

public:
	SendFile(QWidget *parent = Q_NULLPTR);
	~SendFile();

signals:	//�����ļ�������ź�
	void sendFileClicked(QString &QstrData, int &msgType, QString fileName);
private slots:
	void on_openBtn_clicked();	//���ļ���ť���
	void on_sendBtn_clicked();	//���Ͱ�ť���


private:
	Ui::SendFile ui;
	QString m_filePath;	//ѡ����ļ�·��
};
