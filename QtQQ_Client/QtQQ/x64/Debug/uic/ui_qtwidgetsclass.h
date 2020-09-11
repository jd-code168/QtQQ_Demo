/********************************************************************************
** Form generated from reading UI file 'qtwidgetsclass.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTWIDGETSCLASS_H
#define UI_QTWIDGETSCLASS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtWidgetsClass
{
public:

    void setupUi(QWidget *QtWidgetsClass)
    {
        if (QtWidgetsClass->objectName().isEmpty())
            QtWidgetsClass->setObjectName(QString::fromUtf8("QtWidgetsClass"));
        QtWidgetsClass->resize(400, 300);

        retranslateUi(QtWidgetsClass);

        QMetaObject::connectSlotsByName(QtWidgetsClass);
    } // setupUi

    void retranslateUi(QWidget *QtWidgetsClass)
    {
        QtWidgetsClass->setWindowTitle(QCoreApplication::translate("QtWidgetsClass", "QtWidgetsClass", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtWidgetsClass: public Ui_QtWidgetsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTWIDGETSCLASS_H
