#ifndef PLANTGENGUI_H
#define PLANTGENGUI_H

#include <QWidget>
#include <QPushButton>
#include <QGraphicsOpacityEffect>
#include <QMediaPlayer>

#include "PlantDatabase.h"

namespace Ui {
class PlantGenGUI;
}

class PlantGenGUI : public QWidget
{
    Q_OBJECT

public slots:
    void updatePlantAbs(int);
    void updatePlantImage();
public:
    explicit PlantGenGUI(QWidget *parent = 0);
    ~PlantGenGUI();

    static PlantGenGUI *pGUI;
    PlantDatabase::Abilities curPlant;

    QPushButton * getGUISwitchBtn();

    void playMusic();
    void stopMusic();

    void setTestLabelText( std::string text );
    void getPlants( int p1, int p2, int p3 );

private:
    void getIndexesAndBias(int& l_index, int& r_index, float bias, int ability);

    Ui::PlantGenGUI *ui;
    PlantDatabase pdb;
    std::vector<PlantDatabase::PlantData> plants;

    QMediaPlayer mPlayer;
    QMediaPlayer sePlayer;

    //temp
    int labelLines;

    // opacity effects for the icons
    QGraphicsOpacityEffect opFxSun;
    QGraphicsOpacityEffect opFxThorns;
    QGraphicsOpacityEffect opFxSkull;
    QGraphicsOpacityEffect opFxNose;
    QGraphicsOpacityEffect opFxFruit;
    QGraphicsOpacityEffect opFxToy;
    QGraphicsOpacityEffect opFxTree;
    QGraphicsOpacityEffect opFxRain;

   std::vector<unsigned char> m_img;
};

#endif // PLANTGENGUI_H
