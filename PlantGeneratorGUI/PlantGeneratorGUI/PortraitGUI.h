#ifndef PORTRAITGUI_H
#define PORTRAITGUI_H

#include <QWidget>

namespace Ui {
class PortraitGUI;
}

class PortraitGUI : public QWidget
{
    Q_OBJECT

public:
    explicit PortraitGUI(QWidget *parent = 0);
    ~PortraitGUI();

private:
    Ui::PortraitGUI *ui;
};

#endif // PORTRAITGUI_H
