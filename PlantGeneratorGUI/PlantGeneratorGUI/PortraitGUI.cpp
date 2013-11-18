
#include <iostream>

#include "PortraitGUI.h"
#include "ui_PortraitGUI.h"

PortraitGUI::PortraitGUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PortraitGUI)
{
    ui->setupUi(this);

    std::cout << "PortraitGUI" << std::endl;
}

PortraitGUI::~PortraitGUI()
{
    delete ui;
}
