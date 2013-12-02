#include "videohandler.h"

#include <QMediaService>
#include <QMediaPlaylist>
#include <QVideoProbe>
#include <QDebug>

VideoHandler::VideoHandler(QWidget *parent) :
    QStackedWidget(parent)
{
    player = new QMediaPlayer(this, QMediaPlayer::VideoSurface);
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
   playlist = new QMediaPlaylist(player);
   player->setPlaylist(playlist);
   playlist->setCurrentIndex(1);
   open();
   //addToPlaylist(fileNames);
   this->setCurrentWidget(_movieWidget);

   player->play();
}

void VideoHandler::GoToPortrait()
{

    if(player->mediaStatus() == QMediaPlayer::InvalidMedia)
    {
        //_errorMessage->setText("invalid media");
        this->setCurrentWidget(_noMovieWidget);
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
        this->setCurrentWidget(_noMovieWidget);
    }
    else if(player->mediaStatus() == QMediaPlayer::EndOfMedia)
    {
        this->setCurrentWidget(_noMovieWidget);
    }
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
