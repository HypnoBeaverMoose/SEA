#include "videohandler.h"

#include <QMediaService>
#include <QMediaPlaylist>
#include <QVideoProbe>
#include <QDebug>

VideoHandler::VideoHandler(QWidget *parent) :
    QStackedWidget(parent), player(0), playlist(0)
{
    player = new QMediaPlayer(this, QMediaPlayer::VideoSurface);
}


VideoHandler::~VideoHandler()
{
    if ( player )
        delete player;
    if ( playlist )
        delete playlist;
}


void VideoHandler::handleError()
{
    _errorMessage->setText("Error: " + player->errorString());
}

void VideoHandler::SetVariables(QWidget *noMoviePage, QVideoWidget *moviePage, QLabel *errorMessage)
{
    _noMovieWidget = noMoviePage;
    _movieWidget = moviePage;
    _errorMessage = errorMessage;

    this->setCurrentWidget(_movieWidget); // set the variables of the widget
    this->setCurrentWidget(_noMovieWidget); // set the variables of the widget

    //videoWidget = new QVideoWidget(_movieWidget);
    player->setVideoOutput(_movieWidget);
    //videoWidget->setGeometry(QRect(_movieWidget->y(), _movieWidget->x(),_movieWidget->width(), _movieWidget->height()));

    connect(player, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(handleError()));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(GoToPortrait()) );
}

void VideoHandler::PlayMovie(const QStringList& fileNames)
{
    if ( playlist )
    {
        delete playlist;
    }
   playlist = new QMediaPlaylist(player);
   player->setPlaylist(playlist);
   playlist->setCurrentIndex(-1);
   //open();
   addToPlaylist(fileNames);
    //player->setMedia(QUrl::fromLocalFile("/sdcard/Music/test.mp3"));
    //player->setVolume(100);
   //this->setCurrentWidget(_movieWidget);

   player->play();
}

void VideoHandler::GoToPortrait()
{
    if(player->mediaStatus() == QMediaPlayer::InvalidMedia)
    {
        _errorMessage->setText("invalid media");
        ExitMovie();
    }
    else if(player->mediaStatus() == QMediaPlayer::NoMedia)
    {
        if(player->playlist()->isEmpty())
        {
            _errorMessage->setText("no media");
        }
        else
        {
            _errorMessage->setText("media");
        }
        ExitMovie();
    }
    else if(player->mediaStatus() == QMediaPlayer::EndOfMedia)
    {
        _errorMessage->setText("end");
        //if(player->playlist()->currentIndex() >= player->playlist()->mediaCount())
        //{
            ExitMovie();
        //}
     }
    /*else if(player->state() == QMediaPlayer::PausedState)
    {
        _errorMessage->setText("paused");
        ExitMovie();
    }
    else
    {
        if(player->mediaStatus() == QMediaPlayer::UnknownMediaStatus)
        {
            _errorMessage->setText("unknown media status");
        }
        else if(player->mediaStatus() == QMediaPlayer::LoadedMedia)
        {
            _errorMessage->setText("loaded media");
        }
        else if(player->mediaStatus() == QMediaPlayer::StalledMedia)
        {
            _errorMessage->setText("stalled");
        }
        else if(player->mediaStatus() == QMediaPlayer::BufferedMedia)
        {
            _errorMessage->setText("buffered");
        }
        ExitMovie();
    }*/
}

void VideoHandler::ExitMovie()
{
    player->stop();
    this->setCurrentWidget(_noMovieWidget);
}

void VideoHandler::open()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open Files"));

    addToPlaylist(fileNames);
}

void VideoHandler::addToPlaylist(const QStringList& fileNames)
{
    foreach (QString const &argument, fileNames) {
        QFileInfo fileInfo(argument);
        if (fileInfo.exists()) {
            QUrl url = QUrl::fromLocalFile(fileInfo.absoluteFilePath());
            if (fileInfo.suffix().toLower() == QLatin1String("m3u")) {
                playlist->load(url);
            } else
            {
                playlist->addMedia(url);
                _errorMessage->setText(argument);
            }
        } else {
            QUrl url(argument);
            if (url.isValid()) {
                playlist->addMedia(url);
            }
        }
    }
}
