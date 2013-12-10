#ifndef SWITCHWINDOW_H
#define SWITCHWINDOW_H

#include <QStackedWidget>

#include "PlantGenGUI.h"
#include "PortraitGUI.h"


namespace Ui {
class SwitchWindow;
}

class SwitchWindow : public QStackedWidget
{
    Q_OBJECT

public slots:
    void toggleGUI();
    void loadPlants();
public:
    explicit SwitchWindow(QWidget *parent = 0);
    ~SwitchWindow();
    void AfterShownSetVariables();

private:
    Ui::SwitchWindow *ui;
    PlantGenGUI genGUI;
    PortraitGUI portGUI;
    QMediaPlayer buttonPlayer;

    int plantGenIdx;
    int portraitIdx;
};

#endif // SWITCHWINDOW_H
