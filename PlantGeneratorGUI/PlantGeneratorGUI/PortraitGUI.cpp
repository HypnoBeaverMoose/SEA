#include <iostream>
#include "PlantGenGUI.h"
#include "PortraitGUI.h"
#include "ui_PortraitGUI.h"
#include "Player.h"
#include <QStringList>
#include <QPixmap>
#include <QFile>
#include <QFontDatabase>

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

    if (!portrait.load(":/Portrait/Images/portrait.jpg"))
        std::cout << "Error loading image :/Portrait/Images/portrait.jpg" << std::endl;

    QFile fontFile(":/Portrait/Font/goodfishbd.ttf");
    if (!fontFile.open(QIODevice::ReadOnly))
        std::cout << "failed to open font file" << std::endl;
    QByteArray fontData = fontFile.readAll();

    if (QFontDatabase::addApplicationFontFromData(fontData) == -1)
        std::cout << "failed to add a font" << std::endl;
    QFont font("goodfish", 28, QFont::Normal);

    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(9);
    effect->setColor(QColor(0, 0, 0, 255));
    effect->setOffset(0, 0);
    ui->SubtitleMessage->setGraphicsEffect(effect);
    ui->SubtitleMessage->setFont(font);
    resetPainting();

    ui->guiSwitchBtn->setImages( &btnImg, &btnImg_down );

    meikePlayer = new QMediaPlayer();
    connect( ui->guiSwitchBtn, SIGNAL(clicked()), this, SLOT(Exit()) );
    connect(meikePlayer, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(MediaStatusChanged()) );

    //setup background music
    mPlayer.setMedia( QUrl("assets:/meikeMusic.mp3") );
    mPlayer.setVolume(60);
    connect(&mPlayer, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(RepeatMusic()) );

}

void PortraitGUI::AfterShownSetVariables()
{
    //ui->MovieView->SetVariables(ui->portraitPage, ui->videoPage, ui->SubtitleMessage);
    //PlayMovies();
}

void PortraitGUI::PlayMovies()
{
    meikePlayer->stop();
    //bool call = true;
    int plants[3];
    for(int i = 0; i < 3; i++){
        plants[i] = PlantGenGUI::pGUI->plants[i].id;
        //call == call && (plants[i] > 0);
    }

    //if(call)
        playList = player.PlayDialogue(PlantGenGUI::pGUI->curPlant, plants, PlantGenGUI::pGUI->plantGenerated);
        player.outputText(playList);
    PlayMeikeSound();
}

void PortraitGUI::RepeatMusic()
{
    if(mPlayer.mediaStatus() == QMediaPlayer::MediaStatus::EndOfMedia)
    {
        playMusic();
    }
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
    //ui->MovieView->ExitMovie();
}

void PortraitGUI::MediaStatusChanged()
{
    switch(meikePlayer->mediaStatus())
    {
        case QMediaPlayer::MediaStatus::EndOfMedia:
        PlayMeikeSound();
        break;
        default:
        break;
    }
}

void PortraitGUI::PlayMeikeSound()
{
    if(playList.size() > 0)
    {
        QString fileName;
        fileName = "assets:/sounds/" + QString(playList[0].source.c_str()) + ".wav";
        ui->SubtitleMessage->setText(QString(playList[0].dialogue.c_str()));
        meikePlayer->setMedia(QUrl(fileName));
        meikePlayer->play();
        fileName = "assets:/Stills/" + QString(playList[0].source.c_str()) + ".jpg";
        QPixmap px;
        if(!px.load(fileName)){resetPainting();}
        else{ui->portrait->setPixmap(px);}
        playList.erase(playList.begin());
    }
    else
    {
        resetPainting();
    }
}

void PortraitGUI::resetPainting()
{
    ui->portrait->setPixmap(portrait);
    ui->SubtitleMessage->setText("");
}

PortraitGUI::~PortraitGUI()
{
    if ( meikePlayer )
        delete meikePlayer;
}

QPushButton * PortraitGUI::getGUISwitchBtn()
{
    return ui->guiSwitchBtn;
}
