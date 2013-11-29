
#include <iostream>

#include "MainWindow.h"
#include "ui_MainWindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), db()
{
    ui->setupUi(this);

    // database test
    std::cout << "Hello database world.." << std::endl;

    QObject::connect( ui->addPlantBtn, SIGNAL(clicked()), this, SLOT(onAddPlantClicked()) );
    QObject::connect( ui->getPlantBtn, SIGNAL(clicked()), this, SLOT(onGetPlantClicked()) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAddPlantClicked()
{
    std::cout << "adding plant.." << std::endl;

    PlantDatabase::PlantData newPlant;
    newPlant.id        = 66;
    newPlant.angle     = 0.1f;
    newPlant.scale     = 0.2f;
    newPlant.angleInc  = 0.3f;
    newPlant.scaleInc  = 0.4f;
    newPlant.iterCount = 5;
    newPlant.axiom     = "newPlant";

    int i;
    // add dummy DrawableObject data
    for ( i = 0; i < 5; ++i )
    {
        PlantDatabase::DrawData dData;
        dData.clr        = "#AABBCCDD";
        dData.letter     = 'a';
        dData.vertOffset = float(i);

        for ( int j = 0; j < 10; ++j )
            dData.verts.push_back( PlantDatabase::Point(float(j), float(j) + 0.5f) );

        newPlant.drawData.push_back(dData);
    }

    // add dummy rules
    for ( i = 0; i < 3; ++i )
    {
        PlantDatabase::LRule r;
        r.prob = float(3) / 3;
        r.lhs  = 'a';
        r.rhs  = "aa//##[a][a][a]";

        newPlant.rules.push_back(r);
    }

    db.addPlant( newPlant );
}


void MainWindow::onGetPlantClicked()
{
    std::cout << "retrieving plant 66.." << std::endl;

    bool res = true;
    db.getPlant(66, res);
    std::cout << "plant found: " << res << std::endl;
}

