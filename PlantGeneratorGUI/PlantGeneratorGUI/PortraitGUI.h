#ifndef PORTRAITGUI_H
#define PORTRAITGUI_H

#include <QWidget>
#include <QPushButton>
#include <QMediaPlayer>

#include "Player.h"

namespace Ui {
class PortraitGUI;
}

class PortraitGUI : public QWidget
{
    Q_OBJECT

public:
    explicit PortraitGUI(QWidget *parent = 0);
    ~PortraitGUI();
    void AfterShownSetVariables();
    void PlayMovies();
    void playMusic();
    void stopMusic();

    QPushButton * getGUISwitchBtn();

private:
    Ui::PortraitGUI *ui;
    QMediaPlayer mPlayer;
    Dialogue::Player player;
    QMediaPlaylist *meikePlaylist;
    QMediaPlayer *meikePlayer;
    //std::map<int, Dialogue::Player> players;
    //Dialogue::Player *GetPlayer(int playerID);

public slots:
    void Exit();
};

#endif // PORTRAITGUI_H
