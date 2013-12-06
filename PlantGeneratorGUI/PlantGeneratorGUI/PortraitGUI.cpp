
#include <iostream>

#include "PortraitGUI.h"
#include "ui_PortraitGUI.h"
#include <QStringList>

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

    connect( ui->guiSwitchBtn, SIGNAL(clicked()), this, SLOT(Exit()));
}

void PortraitGUI::AfterShownSetVariables()
{
    ui->MovieView->SetVariables(ui->portraitPage, ui->videoPage, ui->ErrorMessage);
}

void PortraitGUI::PlayMovies()
{
    QStringList qs;
    qs << "/sdcard/Music/test.mp3";
    ui->MovieView->PlayMovie(qs);
}

void PortraitGUI::Exit()
{
    ui->MovieView->ExitMovie();
}

PortraitGUI::~PortraitGUI()
{
}

QPushButton * PortraitGUI::getGUISwitchBtn()
{
    return ui->guiSwitchBtn;
}
