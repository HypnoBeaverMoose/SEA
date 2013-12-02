#ifndef VIDEOHANDLER_H
#define VIDEOHANDLER_H

#include <QWidget>
#include <QtWidgets>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>

class VideoHandler : public QStackedWidget
{
    Q_OBJECT
public:
    explicit VideoHandler(QWidget *parent = 0);
    void SetVariables(QWidget *noMoviePage, QVideoWidget *moviePage, QLabel *errorMessage);
    void PlayMovie(const QStringList& fileNames);
    void ExitMovie();

signals:

public slots:
    void GoToPortrait();
    void handleError();

private:
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    QVideoWidget *_movieWidget;
    QWidget *_noMovieWidget;
    QLabel *_errorMessage;
    void open();
    void addToPlaylist(const QStringList& fileNames);
};

#endif // VIDEOHANDLER_H
