#ifndef PLANTGENGUI_H
#define PLANTGENGUI_H

#include <QWidget>
#include <QPushButton>
#include <QGraphicsOpacityEffect>

#include "PlantDatabase.h"

namespace Ui {
class PlantGenGUI;
}

class PlantGenGUI : public QWidget
{
    Q_OBJECT

public slots:
    void updateIcons(int);

public:
    explicit PlantGenGUI(QWidget *parent = 0);
    ~PlantGenGUI();

    QPushButton * getGUISwitchBtn();

private:
    void getPlants( int p1, int p2, int p3 );

    Ui::PlantGenGUI *ui;
    PlantDatabase pdb;
    std::vector<PlantDatabase::PlantData> plants;

    // opacity effects for the icons
    QGraphicsOpacityEffect opFxSun;
    QGraphicsOpacityEffect opFxThorns;
    QGraphicsOpacityEffect opFxSkull;
    QGraphicsOpacityEffect opFxNose;
    QGraphicsOpacityEffect opFxFruit;
    QGraphicsOpacityEffect opFxToy;
    QGraphicsOpacityEffect opFxTree;
    QGraphicsOpacityEffect opFxRain;
};

#endif // PLANTGENGUI_H
