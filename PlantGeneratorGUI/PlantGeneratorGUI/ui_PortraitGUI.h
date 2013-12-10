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
#include "qvideowidget.h"
#include "videohandler.h"

QT_BEGIN_NAMESPACE

class Ui_PortraitGUI
{
public:
    VideoHandler *MovieView;
    QWidget *portraitPage;
    QLabel *portrait;
    QLabel *bgImg;
    MaskedButton *guiSwitchBtn;
    QLabel *ErrorMessage;
    QVideoWidget *videoPage;

    void setupUi(QWidget *PortraitGUI)
    {
        if (PortraitGUI->objectName().isEmpty())
            PortraitGUI->setObjectName(QStringLiteral("PortraitGUI"));
        PortraitGUI->resize(800, 1280);
        MovieView = new VideoHandler(PortraitGUI);
        MovieView->setObjectName(QStringLiteral("MovieView"));
        MovieView->setGeometry(QRect(0, 0, 800, 1280));
        portraitPage = new QWidget();
        portraitPage->setObjectName(QStringLiteral("portraitPage"));
        portraitPage->setAutoFillBackground(false);
        portrait = new QLabel(portraitPage);
        portrait->setObjectName(QStringLiteral("portrait"));
        portrait->setGeometry(QRect(5, 2, 791, 1281));
        portrait->setPixmap(QPixmap(QString::fromUtf8(":/Portrait/Images/FarmerGirl_5.png")));
        bgImg = new QLabel(portraitPage);
        bgImg->setObjectName(QStringLiteral("bgImg"));
        bgImg->setGeometry(QRect(0, 0, 800, 1280));
        bgImg->setPixmap(QPixmap(QString::fromUtf8(":/Portrait/Images/portraitBG.png")));
        guiSwitchBtn = new MaskedButton(portraitPage);
        guiSwitchBtn->setObjectName(QStringLiteral("guiSwitchBtn"));
        guiSwitchBtn->setGeometry(QRect(668, 9, 122, 122));
        ErrorMessage = new QLabel(portraitPage);
        ErrorMessage->setObjectName(QStringLiteral("ErrorMessage"));
        ErrorMessage->setGeometry(QRect(40, 60, 721, 1151));
        QFont font;
        font.setPointSize(12);
        ErrorMessage->setFont(font);
        ErrorMessage->setWordWrap(true);
        MovieView->addWidget(portraitPage);
        videoPage = new QVideoWidget();
        videoPage->setObjectName(QStringLiteral("videoPage"));
        MovieView->addWidget(videoPage);

        retranslateUi(PortraitGUI);

        MovieView->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(PortraitGUI);
    } // setupUi

    void retranslateUi(QWidget *PortraitGUI)
    {
        PortraitGUI->setWindowTitle(QApplication::translate("PortraitGUI", "Form", 0));
        portrait->setText(QString());
        bgImg->setText(QString());
        guiSwitchBtn->setText(QString());
        ErrorMessage->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class PortraitGUI: public Ui_PortraitGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PORTRAITGUI_H
