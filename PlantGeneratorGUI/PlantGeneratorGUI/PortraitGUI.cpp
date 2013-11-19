
#include <iostream>

#include "PortraitGUI.h"
#include "ui_PortraitGUI.h"

PortraitGUI::PortraitGUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PortraitGUI)
{
    ui->setupUi(this);

    ui->guiSwitchBtn->setSize( QSize(122, 122) );
    QImage btnImg;

    if ( !btnImg.load(":/Portrait/toGenBtn.png") )
        std::cout << "Error loading image" << std::endl;
    ui->guiSwitchBtn->setImages( &btnImg, &btnImg );
}

PortraitGUI::~PortraitGUI()
{
    delete ui;
}

QPushButton * PortraitGUI::getGUISwitchBtn()
{
    return ui->guiSwitchBtn;
}
