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

public:
    explicit SwitchWindow(QWidget *parent = 0);
    ~SwitchWindow();

private:
    Ui::SwitchWindow *ui;
    PlantGenGUI genGUI;
    PortraitGUI portGUI;

    int plantGenIdx;
    int portraitIdx;
};

#endif // SWITCHWINDOW_H
