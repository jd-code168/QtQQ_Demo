#pragma once

#include <QWidget>
#include "ui_userlogin.h"

#include "basicwindow.h"

class UserLogin : public BasicWindow
{
	Q_OBJECT

public:
	UserLogin(QWidget *parent = Q_NULLPTR);
	~UserLogin();

private slots:
	void onLoginBtnClicked();

protected:
	void initControl();
	bool connectMySql();
	bool verfyAccountCode(bool isAccountLogin,QString &strAccount);

private:
	Ui::UserLogin ui;
};
