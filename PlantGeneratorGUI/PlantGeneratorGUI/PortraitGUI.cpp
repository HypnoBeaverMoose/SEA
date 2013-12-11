
#include <iostream>
#include "PlantGenGUI.h"
#include "PortraitGUI.h"
#include "ui_PortraitGUI.h"
#include "Player.h"
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

    meikePlayer = new QMediaPlayer();
    meikePlaylist = new QMediaPlaylist();
<<<<<<< HEAD
    meikePlayer->setPlaylist(meikePlaylist);
=======
>>>>>>> robin edits
    connect( ui->guiSwitchBtn, SIGNAL(clicked()), this, SLOT(Exit()) );
    connect(meikePlayer, SIGNAL(stateChanged(QMediaPlayer::MediaStatus)), this, SLOT(MediaStatusChanged()) );

    //setup background music
    mPlayer.setMedia( QUrl("assets:/meikeMusic.mp3") );
    mPlayer.setVolume(100);

}

void PortraitGUI::AfterShownSetVariables()
{
    ui->MovieView->SetVariables(ui->portraitPage, ui->videoPage, ui->ErrorMessage);
    PlayMovies();
}

void PortraitGUI::PlayMovies()
{
<<<<<<< HEAD
   meikePlayer->stop();
    bool call = true;
    int plants[3];
    for(int i = 0; i < 3; i++){
        plants[i] = PlantGenGUI::pGUI->plants[i].id;
        call == call && (plants[i] > 0);
    }
   std::vector<Dialogue::Player::DialogueStruct>  vec;

    if(call)
        vec = player.PlayDialogue(PlantGenGUI::pGUI->curPlant, plants, PlantGenGUI::pGUI->plantGenerated);

    qDebug()<<"DialogueStruct vector size: "<<vec.size();


    QString string;
    meikePlaylist->clear();
    for(int i = 0; i < vec.size(); i++)
    {
        string +=QString(vec[i].dialogue.c_str());
        QString fileName;
        fileName = "assets:/sounds/" + QString(vec[i].source.c_str());
        meikePlaylist->addMedia(QUrl(fileName));
    }

    ui->ErrorMessage->setText(string);
    meikePlaylist->setCurrentIndex(1);
    meikePlayer->play();

=======
    static bool var = false;
    if(!var){
    std::vector<Dialogue::Player::DialogueStruct> ds;
    Dialogue::Player::DialogueStruct newds;
    newds.dialogue = "c01.wav";
    ds.push_back(newds);
    newds.dialogue = "c02.wav";
    ds.push_back(newds);
    newds.dialogue = "c03.wav";
    ds.push_back(newds);

    meikePlayer->stop();
    meikePlaylist = new QMediaPlaylist(meikePlayer);
    for(int i = 0; i < ds.size(); i++)
    {
        qDebug()<<"movie"<<i;
        QString fileName;
        fileName = "assets:/sounds/" + QString(ds[i].dialogue.c_str());
        meikePlaylist->addMedia(QUrl(fileName));
    }
    meikePlaylist->setCurrentIndex(1);
    meikePlayer->setPlaylist(meikePlaylist);
    var = true;
    }
    meikePlayer->play();
>>>>>>> robin edits
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

void PortraitGUI::MediaStatusChanged()
{
    qDebug()<<"change media";
    //PlayMeikeSound();
    switch(meikePlayer->mediaStatus())
    {
        case QMediaPlayer::MediaStatus::EndOfMedia:

        qDebug()<<"change media 2222";
        PlayMeikeSound();
        break;
    }
}

void PortraitGUI::PlayMeikeSound()
{
              qDebug()<<"play meike";
    /*if(playList.size() > 0)
    {
              qDebug()<<"size";
        meikePlayer->setMedia(QUrl(playList[0]));
        playList.erase(playList.begin());
        meikePlayer->play();

        qDebug()<<"play again";
    }*/
}

PortraitGUI::~PortraitGUI()
{
    if ( meikePlayer )
        delete meikePlayer;
    if ( meikePlaylist )
        delete meikePlaylist;
}

QPushButton * PortraitGUI::getGUISwitchBtn()
{
    return ui->guiSwitchBtn;
}
