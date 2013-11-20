#ifndef PORTRAITGUI_H
#define PORTRAITGUI_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class PortraitGUI;
}

class PortraitGUI : public QWidget
{
    Q_OBJECT

public:
    explicit PortraitGUI(QWidget *parent = 0);
    ~PortraitGUI();

    QPushButton * getGUISwitchBtn();

private:
    Ui::PortraitGUI *ui;
};

#endif // PORTRAITGUI_H
