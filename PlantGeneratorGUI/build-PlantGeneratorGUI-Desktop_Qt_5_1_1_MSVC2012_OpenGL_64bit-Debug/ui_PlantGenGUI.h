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
    QLabel *iconApple;
    QLabel *iconToy;
    QLabel *iconTree;
    QLabel *iconRain;
    QLabel *iconNose;
    QLabel *iconThorns;
    QLabel *iconSkull;
    QLabel *iconSun;
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
        dialStalk->setGeometry(QRect(239, -10, 271, 291));
        dialStalk->setAutoFillBackground(false);
        dialStalk->setStyleSheet(QStringLiteral(""));
        dialStalk->setInvertedAppearance(false);
        dialStalk->setInvertedControls(false);
        dialStalk->setWrapping(true);
        dialStalk->setNotchesVisible(false);
        dialLeaf = new ArrowDial(centralWidget);
        dialLeaf->setObjectName(QStringLiteral("dialLeaf"));
        dialLeaf->setGeometry(QRect(500, 140, 271, 291));
        dialLeaf->setAutoFillBackground(false);
        dialLeaf->setStyleSheet(QStringLiteral(""));
        dialLeaf->setInvertedAppearance(false);
        dialLeaf->setInvertedControls(false);
        dialLeaf->setWrapping(true);
        dialLeaf->setNotchesVisible(false);
        dialFlower = new ArrowDial(centralWidget);
        dialFlower->setObjectName(QStringLiteral("dialFlower"));
        dialFlower->setGeometry(QRect(-20, 139, 271, 291));
        dialFlower->setAutoFillBackground(false);
        dialFlower->setStyleSheet(QStringLiteral(""));
        dialFlower->setInvertedAppearance(false);
        dialFlower->setInvertedControls(false);
        dialFlower->setWrapping(true);
        dialFlower->setNotchesVisible(false);
        plant1Label = new QLabel(centralWidget);
        plant1Label->setObjectName(QStringLiteral("plant1Label"));
        plant1Label->setGeometry(QRect(141, 1165, 163, 59));
        QFont font;
        font.setFamily(QStringLiteral("Pristina"));
        font.setPointSize(18);
        plant1Label->setFont(font);
        plant1Label->setAlignment(Qt::AlignCenter);
        plant2Label = new QLabel(centralWidget);
        plant2Label->setObjectName(QStringLiteral("plant2Label"));
        plant2Label->setGeometry(QRect(320, 1165, 163, 59));
        plant2Label->setFont(font);
        plant2Label->setAlignment(Qt::AlignCenter);
        plant3Label = new QLabel(centralWidget);
        plant3Label->setObjectName(QStringLiteral("plant3Label"));
        plant3Label->setGeometry(QRect(495, 1165, 163, 59));
        plant3Label->setFont(font);
        plant3Label->setAlignment(Qt::AlignCenter);
        glowThorns = new QLabel(centralWidget);
        glowThorns->setObjectName(QStringLiteral("glowThorns"));
        glowThorns->setEnabled(true);
        glowThorns->setGeometry(QRect(-16, 622, 177, 205));
        glowThorns->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/iconGlow.png")));
        iconApple = new QLabel(centralWidget);
        iconApple->setObjectName(QStringLiteral("iconApple"));
        iconApple->setGeometry(QRect(702, 800, 76, 102));
        iconApple->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/iconApple.png")));
        iconToy = new QLabel(centralWidget);
        iconToy->setObjectName(QStringLiteral("iconToy"));
        iconToy->setGeometry(QRect(702, 669, 76, 102));
        iconToy->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/iconToy.png")));
        iconTree = new QLabel(centralWidget);
        iconTree->setObjectName(QStringLiteral("iconTree"));
        iconTree->setGeometry(QRect(701, 934, 76, 102));
        iconTree->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/iconTree.png")));
        iconRain = new QLabel(centralWidget);
        iconRain->setObjectName(QStringLiteral("iconRain"));
        iconRain->setGeometry(QRect(702, 536, 76, 102));
        iconRain->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/iconRain.png")));
        iconNose = new QLabel(centralWidget);
        iconNose->setObjectName(QStringLiteral("iconNose"));
        iconNose->setGeometry(QRect(21, 933, 76, 102));
        iconNose->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/iconNose.png")));
        iconThorns = new QLabel(centralWidget);
        iconThorns->setObjectName(QStringLiteral("iconThorns"));
        iconThorns->setGeometry(QRect(20, 667, 76, 102));
        iconThorns->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/iconThorns.png")));
        iconSkull = new QLabel(centralWidget);
        iconSkull->setObjectName(QStringLiteral("iconSkull"));
        iconSkull->setGeometry(QRect(21, 798, 76, 102));
        iconSkull->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/iconSkull.png")));
        iconSun = new QLabel(centralWidget);
        iconSun->setObjectName(QStringLiteral("iconSun"));
        iconSun->setGeometry(QRect(19, 536, 76, 102));
        iconSun->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/iconSun.png")));
        glowSkull = new QLabel(centralWidget);
        glowSkull->setObjectName(QStringLiteral("glowSkull"));
        glowSkull->setGeometry(QRect(-16, 751, 169, 210));
        glowSkull->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/iconGlow.png")));
        glowSun = new QLabel(centralWidget);
        glowSun->setObjectName(QStringLiteral("glowSun"));
        glowSun->setGeometry(QRect(-16, 487, 183, 208));
        glowSun->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/iconGlow.png")));
        glowNose = new QLabel(centralWidget);
        glowNose->setObjectName(QStringLiteral("glowNose"));
        glowNose->setGeometry(QRect(-16, 889, 158, 201));
        glowNose->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/iconGlow.png")));
        glowRain = new QLabel(centralWidget);
        glowRain->setObjectName(QStringLiteral("glowRain"));
        glowRain->setGeometry(QRect(665, 491, 191, 200));
        glowRain->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/iconGlow.png")));
        glowApple = new QLabel(centralWidget);
        glowApple->setObjectName(QStringLiteral("glowApple"));
        glowApple->setGeometry(QRect(665, 755, 186, 202));
        glowApple->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/iconGlow.png")));
        glowTree = new QLabel(centralWidget);
        glowTree->setObjectName(QStringLiteral("glowTree"));
        glowTree->setGeometry(QRect(665, 886, 179, 206));
        glowTree->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/iconGlow.png")));
        glowToy = new QLabel(centralWidget);
        glowToy->setObjectName(QStringLiteral("glowToy"));
        glowToy->setGeometry(QRect(665, 620, 198, 208));
        glowToy->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/iconGlow.png")));
        flowerPlant1 = new QLabel(centralWidget);
        flowerPlant1->setObjectName(QStringLiteral("flowerPlant1"));
        flowerPlant1->setGeometry(QRect(95, 326, 97, 99));
        flowerPlant1->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/stalkCactus.png")));
        flowerPlant2 = new QLabel(centralWidget);
        flowerPlant2->setObjectName(QStringLiteral("flowerPlant2"));
        flowerPlant2->setGeometry(QRect(38, 213, 97, 99));
        flowerPlant2->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/stalkCactus.png")));
        flowerPlant3 = new QLabel(centralWidget);
        flowerPlant3->setObjectName(QStringLiteral("flowerPlant3"));
        flowerPlant3->setGeometry(QRect(153, 211, 97, 99));
        flowerPlant3->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/stalkCactus.png")));
        stalkPlant3 = new QLabel(centralWidget);
        stalkPlant3->setObjectName(QStringLiteral("stalkPlant3"));
        stalkPlant3->setGeometry(QRect(410, 58, 97, 99));
        stalkPlant3->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/stalkCactus.png")));
        stalkPlant1 = new QLabel(centralWidget);
        stalkPlant1->setObjectName(QStringLiteral("stalkPlant1"));
        stalkPlant1->setGeometry(QRect(352, 171, 97, 99));
        stalkPlant1->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/stalkCactus.png")));
        stalkPlant2 = new QLabel(centralWidget);
        stalkPlant2->setObjectName(QStringLiteral("stalkPlant2"));
        stalkPlant2->setGeometry(QRect(297, 67, 97, 99));
        stalkPlant2->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/stalkCactus.png")));
        leafPlant2 = new QLabel(centralWidget);
        leafPlant2->setObjectName(QStringLiteral("leafPlant2"));
        leafPlant2->setGeometry(QRect(548, 223, 97, 99));
        leafPlant2->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/stalkCactus.png")));
        leafPlant3 = new QLabel(centralWidget);
        leafPlant3->setObjectName(QStringLiteral("leafPlant3"));
        leafPlant3->setGeometry(QRect(668, 213, 97, 99));
        leafPlant3->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/stalkCactus.png")));
        leafPlant1 = new QLabel(centralWidget);
        leafPlant1->setObjectName(QStringLiteral("leafPlant1"));
        leafPlant1->setGeometry(QRect(611, 327, 97, 99));
        leafPlant1->setPixmap(QPixmap(QString::fromUtf8(":/PlantGen/stalkCactus.png")));
        guiSwitchBtn = new MaskedButton(centralWidget);
        guiSwitchBtn->setObjectName(QStringLiteral("guiSwitchBtn"));
        guiSwitchBtn->setGeometry(QRect(668, 9, 122, 122));
        QIcon icon;
        icon.addFile(QStringLiteral(":/PlantGen/toPortraitBtn.png"), QSize(), QIcon::Normal, QIcon::Off);
        guiSwitchBtn->setIcon(icon);
        guiSwitchBtn->setIconSize(QSize(122, 121));
        bgImg->raise();
        plant1Label->raise();
        plant2Label->raise();
        plant3Label->raise();
        glowThorns->raise();
        glowSkull->raise();
        glowSun->raise();
        glowNose->raise();
        glowRain->raise();
        glowApple->raise();
        glowTree->raise();
        glowToy->raise();
        iconNose->raise();
        iconRain->raise();
        iconSkull->raise();
        iconToy->raise();
        iconApple->raise();
        iconTree->raise();
        iconThorns->raise();
        iconSun->raise();
        flowerPlant1->raise();
        flowerPlant2->raise();
        flowerPlant3->raise();
        stalkPlant3->raise();
        stalkPlant1->raise();
        stalkPlant2->raise();
        leafPlant2->raise();
        leafPlant3->raise();
        leafPlant1->raise();
        dialStalk->raise();
        dialFlower->raise();
        dialLeaf->raise();
        guiSwitchBtn->raise();

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
        iconApple->setText(QString());
        iconToy->setText(QString());
        iconTree->setText(QString());
        iconRain->setText(QString());
        iconNose->setText(QString());
        iconThorns->setText(QString());
        iconSkull->setText(QString());
        iconSun->setText(QString());
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
    } // retranslateUi

};

namespace Ui {
    class PlantGenGUI: public Ui_PlantGenGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLANTGENGUI_H
