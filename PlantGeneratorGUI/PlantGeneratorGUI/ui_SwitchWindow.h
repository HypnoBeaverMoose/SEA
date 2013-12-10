/********************************************************************************
** Form generated from reading UI file 'SwitchWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SWITCHWINDOW_H
#define UI_SWITCHWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QStackedWidget>

QT_BEGIN_NAMESPACE

class Ui_SwitchWindow
{
public:

    void setupUi(QStackedWidget *SwitchWindow)
    {
        if (SwitchWindow->objectName().isEmpty())
            SwitchWindow->setObjectName(QStringLiteral("SwitchWindow"));
        SwitchWindow->resize(800, 1280);

        retranslateUi(SwitchWindow);

        SwitchWindow->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(SwitchWindow);
    } // setupUi

    void retranslateUi(QStackedWidget *SwitchWindow)
    {
        SwitchWindow->setWindowTitle(QApplication::translate("SwitchWindow", "StackedWidget", 0));
    } // retranslateUi

};

namespace Ui {
    class SwitchWindow: public Ui_SwitchWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SWITCHWINDOW_H
