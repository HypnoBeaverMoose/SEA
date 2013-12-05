/********************************************************************************
** Form generated from reading UI file 'PlantGenGUI.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLANTGENGUI_H
#define UI_PLANTGENGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include "ArrowDial.h"
#include "MaskedButton.h"

QT_BEGIN_NAMESPACE

class Ui_PlantGenGUI
{
public:
    QWidget *centralWidget;
    QLabel *bgImg;
    ArrowDial *dialStalk;
    ArrowDial *dialLeaf;
    ArrowDial *dialFlower;
    QLabel *plant1Label;
    QLabel *plant2Label;
    QLabel *plant3Label;
    QLabel *glowThorns;
    QLabel *glowSkull;
    QLabel *glowSun;
    QLabel *glowNose;
    QLabel *glowRain;
    QLabel *glowApple;
    QLabel *glowTree;
    QLabel *glowToy;
    QLabel *flowerPlant1;
    QLabel *flowerPlant2;
    QLabel *flowerPlant3;
    QLabel *stalkPlant3;
    QLabel *stalkPlant1;
    QLabel *stalkPlant2;
    QLabel *leafPlant2;
    QLabel *leafPlant3;
    QLabel *leafPlant1;
    MaskedButton *guiSwitchBtn;
    QLabel *testLabel;
    QLabel *imgLabel;

    void setupUi(QWidget *PlantGenGUI)
    {
        if (PlantGenGUI->objectName().isEmpty())
            PlantGenGUI->setObjectName(QStringLiteral("PlantGenGUI"));
        PlantGenGUI->resize(800, 1280);
        centralWidget = new QWidget(PlantGenGUI);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setGeometry(QRect(0, 0, 843, 1271));
        bgImg = new QLabel(centralWidget);
        bgImg->setObjectName(QStringLiteral("bgImg"));
        bgImg->setGeometry(QRect(0, 0, 801, 1281));
        bgImg->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/genBG.png")));
        dialStalk = new ArrowDial(centralWidget);
        dialStalk->setObjectName(QStringLiteral("dialStalk"));
        dialStalk->setGeometry(QRect(239, 780, 271, 291));
        dialStalk->setAutoFillBackground(false);
        dialStalk->setStyleSheet(QStringLiteral(""));
        dialStalk->setInvertedAppearance(false);
        dialStalk->setInvertedControls(false);
        dialStalk->setWrapping(true);
        dialStalk->setNotchesVisible(false);
        dialLeaf = new ArrowDial(centralWidget);
        dialLeaf->setObjectName(QStringLiteral("dialLeaf"));
        dialLeaf->setGeometry(QRect(500, 910, 271, 291));
        dialLeaf->setAutoFillBackground(false);
        dialLeaf->setStyleSheet(QStringLiteral(""));
        dialLeaf->setInvertedAppearance(false);
        dialLeaf->setInvertedControls(false);
        dialLeaf->setWrapping(true);
        dialLeaf->setNotchesVisible(false);
        dialFlower = new ArrowDial(centralWidget);
        dialFlower->setObjectName(QStringLiteral("dialFlower"));
        dialFlower->setGeometry(QRect(-20, 919, 271, 291));
        dialFlower->setAutoFillBackground(false);
        dialFlower->setStyleSheet(QStringLiteral(""));
        dialFlower->setInvertedAppearance(false);
        dialFlower->setInvertedControls(false);
        dialFlower->setWrapping(true);
        dialFlower->setNotchesVisible(false);
        plant1Label = new QLabel(centralWidget);
        plant1Label->setObjectName(QStringLiteral("plant1Label"));
        plant1Label->setGeometry(QRect(206, 120, 163, 59));
        QFont font;
        font.setFamily(QStringLiteral("Pristina"));
        font.setPointSize(18);
        plant1Label->setFont(font);
        plant1Label->setAlignment(Qt::AlignCenter);
        plant2Label = new QLabel(centralWidget);
        plant2Label->setObjectName(QStringLiteral("plant2Label"));
        plant2Label->setGeometry(QRect(314, 181, 163, 59));
        plant2Label->setFont(font);
        plant2Label->setAlignment(Qt::AlignCenter);
        plant3Label = new QLabel(centralWidget);
        plant3Label->setObjectName(QStringLiteral("plant3Label"));
        plant3Label->setGeometry(QRect(425, 119, 163, 59));
        plant3Label->setFont(font);
        plant3Label->setAlignment(Qt::AlignCenter);
        glowThorns = new QLabel(centralWidget);
        glowThorns->setObjectName(QStringLiteral("glowThorns"));
        glowThorns->setEnabled(true);
        glowThorns->setGeometry(QRect(14, 424, 177, 205));
        glowThorns->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/glowThorns.png")));
        glowSkull = new QLabel(centralWidget);
        glowSkull->setObjectName(QStringLiteral("glowSkull"));
        glowSkull->setGeometry(QRect(10, 552, 169, 210));
        glowSkull->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/glowSkull.png")));
        glowSun = new QLabel(centralWidget);
        glowSun->setObjectName(QStringLiteral("glowSun"));
        glowSun->setGeometry(QRect(9, 291, 183, 208));
        glowSun->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/glowSun.png")));
        glowNose = new QLabel(centralWidget);
        glowNose->setObjectName(QStringLiteral("glowNose"));
        glowNose->setGeometry(QRect(7, 694, 158, 201));
        glowNose->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/glowNose.png")));
        glowRain = new QLabel(centralWidget);
        glowRain->setObjectName(QStringLiteral("glowRain"));
        glowRain->setGeometry(QRect(689, 294, 191, 200));
        glowRain->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/glowRain.png")));
        glowApple = new QLabel(centralWidget);
        glowApple->setObjectName(QStringLiteral("glowApple"));
        glowApple->setGeometry(QRect(689, 557, 186, 202));
        glowApple->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/glowApple.png")));
        glowTree = new QLabel(centralWidget);
        glowTree->setObjectName(QStringLiteral("glowTree"));
        glowTree->setGeometry(QRect(690, 691, 179, 206));
        glowTree->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/glowTree.png")));
        glowToy = new QLabel(centralWidget);
        glowToy->setObjectName(QStringLiteral("glowToy"));
        glowToy->setGeometry(QRect(693, 428, 198, 208));
        glowToy->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/glowToy.png")));
        flowerPlant1 = new QLabel(centralWidget);
        flowerPlant1->setObjectName(QStringLiteral("flowerPlant1"));
        flowerPlant1->setGeometry(QRect(95, 1103, 97, 99));
        flowerPlant1->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/stalkCactus.png")));
        flowerPlant2 = new QLabel(centralWidget);
        flowerPlant2->setObjectName(QStringLiteral("flowerPlant2"));
        flowerPlant2->setGeometry(QRect(38, 990, 97, 99));
        flowerPlant2->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/stalkCactus.png")));
        flowerPlant3 = new QLabel(centralWidget);
        flowerPlant3->setObjectName(QStringLiteral("flowerPlant3"));
        flowerPlant3->setGeometry(QRect(153, 988, 97, 99));
        flowerPlant3->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/stalkCactus.png")));
        stalkPlant3 = new QLabel(centralWidget);
        stalkPlant3->setObjectName(QStringLiteral("stalkPlant3"));
        stalkPlant3->setGeometry(QRect(410, 850, 97, 99));
        stalkPlant3->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/stalkCactus.png")));
        stalkPlant1 = new QLabel(centralWidget);
        stalkPlant1->setObjectName(QStringLiteral("stalkPlant1"));
        stalkPlant1->setGeometry(QRect(352, 963, 97, 99));
        stalkPlant1->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/stalkCactus.png")));
        stalkPlant2 = new QLabel(centralWidget);
        stalkPlant2->setObjectName(QStringLiteral("stalkPlant2"));
        stalkPlant2->setGeometry(QRect(297, 859, 97, 99));
        stalkPlant2->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/stalkCactus.png")));
        leafPlant2 = new QLabel(centralWidget);
        leafPlant2->setObjectName(QStringLiteral("leafPlant2"));
        leafPlant2->setGeometry(QRect(548, 1000, 97, 99));
        leafPlant2->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/stalkCactus.png")));
        leafPlant3 = new QLabel(centralWidget);
        leafPlant3->setObjectName(QStringLiteral("leafPlant3"));
        leafPlant3->setGeometry(QRect(668, 990, 97, 99));
        leafPlant3->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/stalkCactus.png")));
        leafPlant1 = new QLabel(centralWidget);
        leafPlant1->setObjectName(QStringLiteral("leafPlant1"));
        leafPlant1->setGeometry(QRect(611, 1104, 97, 99));
        leafPlant1->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/stalkCactus.png")));
        guiSwitchBtn = new MaskedButton(centralWidget);
        guiSwitchBtn->setObjectName(QStringLiteral("guiSwitchBtn"));
        guiSwitchBtn->setGeometry(QRect(668, 9, 122, 122));
        QIcon icon;
        icon.addFile(QStringLiteral(":/PlantGen/toPortraitBtn.png"), QSize(), QIcon::Normal, QIcon::Off);
        guiSwitchBtn->setIcon(icon);
        guiSwitchBtn->setIconSize(QSize(122, 121));
        testLabel = new QLabel(centralWidget);
        testLabel->setObjectName(QStringLiteral("testLabel"));
        testLabel->setGeometry(QRect(205, 350, 370, 487));
        testLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        imgLabel = new QLabel(centralWidget);
        imgLabel->setObjectName(QStringLiteral("imgLabel"));
        imgLabel->setGeometry(QRect(143, 344, 512, 512));
        bgImg->raise();
        guiSwitchBtn->raise();
        testLabel->raise();
        imgLabel->raise();
        plant1Label->raise();
        plant3Label->raise();
        plant2Label->raise();
        flowerPlant2->raise();
        leafPlant1->raise();
        stalkPlant2->raise();
        flowerPlant1->raise();
        flowerPlant3->raise();
        stalkPlant1->raise();
        stalkPlant3->raise();
        leafPlant3->raise();
        leafPlant2->raise();
        dialFlower->raise();
        dialStalk->raise();
        dialLeaf->raise();
        glowSun->raise();
        glowApple->raise();
        glowNose->raise();
        glowSkull->raise();
        glowToy->raise();
        glowRain->raise();
        glowThorns->raise();
        glowTree->raise();

        retranslateUi(PlantGenGUI);

        QMetaObject::connectSlotsByName(PlantGenGUI);
    } // setupUi

    void retranslateUi(QWidget *PlantGenGUI)
    {
        PlantGenGUI->setWindowTitle(QApplication::translate("PlantGenGUI", "PlantGenGUI", 0));
        bgImg->setText(QString());
        plant1Label->setText(QApplication::translate("PlantGenGUI", "Plant1", 0));
        plant2Label->setText(QApplication::translate("PlantGenGUI", "Plant2", 0));
        plant3Label->setText(QApplication::translate("PlantGenGUI", "Plant3", 0));
        glowThorns->setText(QString());
        glowSkull->setText(QString());
        glowSun->setText(QString());
        glowNose->setText(QString());
        glowRain->setText(QString());
        glowApple->setText(QString());
        glowTree->setText(QString());
        glowToy->setText(QString());
        flowerPlant1->setText(QString());
        flowerPlant2->setText(QString());
        flowerPlant3->setText(QString());
        stalkPlant3->setText(QString());
        stalkPlant1->setText(QString());
        stalkPlant2->setText(QString());
        leafPlant2->setText(QString());
        leafPlant3->setText(QString());
        leafPlant1->setText(QString());
        guiSwitchBtn->setText(QString());
        testLabel->setText(QString());
        imgLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class PlantGenGUI: public Ui_PlantGenGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLANTGENGUI_H
