//#include "CCMainWindow.h"
#include <QtWidgets/QApplication>
#include "userlogin.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	/*CCMainWindow w;
	w.show();*/

	a.setQuitOnLastWindowClosed(false);

	UserLogin *userlogin = new UserLogin;
	userlogin->show();

	return a.exec();
}
