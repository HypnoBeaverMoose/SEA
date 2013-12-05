/********************************************************************************
** Form generated from reading UI file 'PortraitGUI.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PORTRAITGUI_H
#define UI_PORTRAITGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include "MaskedButton.h"

QT_BEGIN_NAMESPACE

class Ui_PortraitGUI
{
public:
    MaskedButton *guiSwitchBtn;
    QLabel *bgImg;

    void setupUi(QWidget *PortraitGUI)
    {
        if (PortraitGUI->objectName().isEmpty())
            PortraitGUI->setObjectName(QStringLiteral("PortraitGUI"));
        PortraitGUI->resize(800, 1280);
        guiSwitchBtn = new MaskedButton(PortraitGUI);
        guiSwitchBtn->setObjectName(QStringLiteral("guiSwitchBtn"));
        guiSwitchBtn->setGeometry(QRect(668, 9, 122, 122));
        QIcon icon;
        icon.addFile(QStringLiteral(":/Portrait/toPortraitBtn.png"), QSize(), QIcon::Normal, QIcon::Off);
        guiSwitchBtn->setIcon(icon);
        guiSwitchBtn->setIconSize(QSize(122, 122));
        bgImg = new QLabel(PortraitGUI);
        bgImg->setObjectName(QStringLiteral("bgImg"));
        bgImg->setGeometry(QRect(0, 0, 800, 1280));
        bgImg->setPixmap(QPixmap(QString::fromUtf8(":/Portrait/portraitBG.png")));
        bgImg->raise();
        guiSwitchBtn->raise();

        retranslateUi(PortraitGUI);

        QMetaObject::connectSlotsByName(PortraitGUI);
    } // setupUi

    void retranslateUi(QWidget *PortraitGUI)
    {
        PortraitGUI->setWindowTitle(QApplication::translate("PortraitGUI", "Form", 0));
        guiSwitchBtn->setText(QString());
        bgImg->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class PortraitGUI: public Ui_PortraitGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PORTRAITGUI_H
