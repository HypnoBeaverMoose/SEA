#ifndef PORTRAITGUI_H
#define PORTRAITGUI_H

#include <QWidget>
#include <QPushButton>
#include <QMediaPlayer>

namespace Ui {
class PortraitGUI;
}

class PortraitGUI : public QWidget
{
    Q_OBJECT

public slots:
    void Exit();

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
};

#endif // PORTRAITGUI_H
