
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
    if ( !btnImg.load(":/Portrait/Images/toGenBtn.png") )
        std::cout << "Error loading image" << std::endl;
    QImage btnImg_down;
    if ( !btnImg_down.load(":/Portrait/Images/toGenBtn_down.png") )
        std::cout << "Error loading image" << std::endl;

    ui->guiSwitchBtn->setImages( &btnImg, &btnImg_down );

    connect( ui->guiSwitchBtn, SIGNAL(clicked()), this, SLOT(Exit()) );

    //setup background music
    mPlayer.setMedia( QUrl("assets:/meikeMusic.mp3") );
    mPlayer.setVolume(100);

}

void PortraitGUI::AfterShownSetVariables()
{
    ui->MovieView->SetVariables(ui->portraitPage, ui->videoPage, ui->ErrorMessage);
}

void PortraitGUI::PlayMovies()
{
    int plants[3] = {1,2,3};
    std::vector<Dialogue::Player::DialogueStruct> data =  player.PlayDialogue(plants,1);
    qDebug()<<"DialogueStruct vector size: "<<data.size();
    QString string;
    for(int i = 0; i < data.size(); i++)
        string +=QString(data[i].dialogue.c_str());

    ui->ErrorMessage->setText(string);

}


void PortraitGUI::playMusic()
{
   mPlayer.play();
}


void PortraitGUI::stopMusic()
{
    mPlayer.stop();
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
