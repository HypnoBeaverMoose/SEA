
#include "PlantGenGUI.h"
#include "SwitchWindow.h"
#include "ui_SwitchWindow.h"

SwitchWindow::SwitchWindow(QWidget *parent) :
    QStackedWidget(parent),
    ui(new Ui::SwitchWindow), genGUI(this), portGUI(this)
{
    ui->setupUi(this);

    plantGenIdx = this->addWidget(&genGUI);
    portraitIdx = this->addWidget(&portGUI);
    setCurrentIndex(plantGenIdx);

    QObject::connect( genGUI.getGUISwitchBtn(), SIGNAL(clicked()), this, SLOT(toggleGUI()) );
    QObject::connect( portGUI.getGUISwitchBtn(), SIGNAL(clicked()), this, SLOT(toggleGUI()) );
}

SwitchWindow::~SwitchWindow()
{
    delete ui;
}


void SwitchWindow::toggleGUI()
{
    if ( currentIndex() == plantGenIdx )
        setCurrentIndex(portraitIdx);
    else setCurrentIndex(plantGenIdx);

}
