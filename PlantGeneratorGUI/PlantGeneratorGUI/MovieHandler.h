#ifndef MOVIEHANDLER_H
#define MOVIEHANDLER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>

class MovieHandler : public QWidget
{
    public:
        MovieHandler();
    private:
        QMediaPlayer *player;
        QMediaPlaylist *playlist;
        void open();
        void addToPlaylist(const QStringList& fileNames);
};

#endif // MOVIEHANDLER_H
