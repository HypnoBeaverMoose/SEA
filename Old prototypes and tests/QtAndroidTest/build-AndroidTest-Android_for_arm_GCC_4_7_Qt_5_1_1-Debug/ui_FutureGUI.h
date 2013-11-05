/********************************************************************************
** Form generated from reading UI file 'FutureGUI.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FUTUREGUI_H
#define UI_FUTUREGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>
#include "MaskedButton.h"

QT_BEGIN_NAMESPACE

class Ui_FutureGUIClass
{
public:
    QAction *actionNew;
    QAction *actionExit;
    QWidget *centralWidget;
    QLabel *label;
    MaskedButton *specialBtnR;
    MaskedButton *stalkBtnR;
    MaskedButton *rootBtnR;
    MaskedButton *leafBtnR;
    MaskedButton *leafBtnL;
    MaskedButton *specialBtnL;
    MaskedButton *rootBtnL;
    MaskedButton *stalkBtnL;
    MaskedButton *saveBtn;

    void setupUi(QMainWindow *FutureGUIClass)
    {
        if (FutureGUIClass->objectName().isEmpty())
            FutureGUIClass->setObjectName(QStringLiteral("FutureGUIClass"));
        FutureGUIClass->resize(1024, 722);
        FutureGUIClass->setMinimumSize(QSize(1024, 722));
        FutureGUIClass->setMaximumSize(QSize(1024, 722));
        actionNew = new QAction(FutureGUIClass);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        actionExit = new QAction(FutureGUIClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        centralWidget = new QWidget(FutureGUIClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, -10, 1091, 740));
        label->setFrameShape(QFrame::NoFrame);
        label->setPixmap(QPixmap(QString::fromUtf8(":/FutureGUI/jugenstilBG.png")));
        specialBtnR = new MaskedButton(centralWidget);
        specialBtnR->setObjectName(QStringLiteral("specialBtnR"));
        specialBtnR->setGeometry(QRect(320, 110, 55, 38));
        QIcon icon;
        icon.addFile(QStringLiteral(":/FutureGUI/jugenstilArrowRight.png"), QSize(), QIcon::Normal, QIcon::On);
        specialBtnR->setIcon(icon);
        specialBtnR->setIconSize(QSize(55, 38));
        specialBtnR->setFlat(true);
        stalkBtnR = new MaskedButton(centralWidget);
        stalkBtnR->setObjectName(QStringLiteral("stalkBtnR"));
        stalkBtnR->setGeometry(QRect(320, 430, 55, 38));
        stalkBtnR->setIcon(icon);
        stalkBtnR->setIconSize(QSize(55, 38));
        stalkBtnR->setFlat(true);
        rootBtnR = new MaskedButton(centralWidget);
        rootBtnR->setObjectName(QStringLiteral("rootBtnR"));
        rootBtnR->setGeometry(QRect(320, 590, 55, 38));
        rootBtnR->setIcon(icon);
        rootBtnR->setIconSize(QSize(55, 38));
        rootBtnR->setFlat(true);
        leafBtnR = new MaskedButton(centralWidget);
        leafBtnR->setObjectName(QStringLiteral("leafBtnR"));
        leafBtnR->setGeometry(QRect(320, 270, 55, 38));
        leafBtnR->setIcon(icon);
        leafBtnR->setIconSize(QSize(55, 38));
        leafBtnR->setFlat(true);
        leafBtnL = new MaskedButton(centralWidget);
        leafBtnL->setObjectName(QStringLiteral("leafBtnL"));
        leafBtnL->setGeometry(QRect(20, 270, 55, 38));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/FutureGUI/jugenstilArrowLeft.png"), QSize(), QIcon::Normal, QIcon::On);
        leafBtnL->setIcon(icon1);
        leafBtnL->setIconSize(QSize(55, 38));
        leafBtnL->setFlat(true);
        specialBtnL = new MaskedButton(centralWidget);
        specialBtnL->setObjectName(QStringLiteral("specialBtnL"));
        specialBtnL->setGeometry(QRect(20, 110, 55, 38));
        specialBtnL->setIcon(icon1);
        specialBtnL->setIconSize(QSize(55, 38));
        specialBtnL->setFlat(true);
        rootBtnL = new MaskedButton(centralWidget);
        rootBtnL->setObjectName(QStringLiteral("rootBtnL"));
        rootBtnL->setGeometry(QRect(20, 590, 55, 38));
        rootBtnL->setIcon(icon1);
        rootBtnL->setIconSize(QSize(55, 38));
        rootBtnL->setFlat(true);
        stalkBtnL = new MaskedButton(centralWidget);
        stalkBtnL->setObjectName(QStringLiteral("stalkBtnL"));
        stalkBtnL->setGeometry(QRect(20, 430, 55, 38));
        stalkBtnL->setIcon(icon1);
        stalkBtnL->setIconSize(QSize(55, 38));
        stalkBtnL->setFlat(true);
        saveBtn = new MaskedButton(centralWidget);
        saveBtn->setObjectName(QStringLiteral("saveBtn"));
        saveBtn->setGeometry(QRect(860, 20, 149, 90));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/FutureGUI/jugenstilSaveBtn.png"), QSize(), QIcon::Normal, QIcon::On);
        saveBtn->setIcon(icon2);
        saveBtn->setIconSize(QSize(149, 90));
        saveBtn->setFlat(true);
        FutureGUIClass->setCentralWidget(centralWidget);

        retranslateUi(FutureGUIClass);

        QMetaObject::connectSlotsByName(FutureGUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *FutureGUIClass)
    {
        FutureGUIClass->setWindowTitle(QApplication::translate("FutureGUIClass", "FutureGUI", 0));
        actionNew->setText(QApplication::translate("FutureGUIClass", "New...", 0));
        actionExit->setText(QApplication::translate("FutureGUIClass", "Exit", 0));
        label->setText(QString());
        specialBtnR->setText(QString());
        stalkBtnR->setText(QString());
        rootBtnR->setText(QString());
        leafBtnR->setText(QString());
        leafBtnL->setText(QString());
        specialBtnL->setText(QString());
        rootBtnL->setText(QString());
        stalkBtnL->setText(QString());
        saveBtn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class FutureGUIClass: public Ui_FutureGUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FUTUREGUI_H
