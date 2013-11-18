
#include <QSignalMapper>

#include "PlantGenGUI.h"
#include "SwitchWindow.h"
#include "ui_SwitchWindow.h"

SwitchWindow::SwitchWindow(QWidget *parent) :
    QStackedWidget(parent),
    ui(new Ui::SwitchWindow), genGUI(this), portGUI(this)
{
    ui->setupUi(this);

    int idxGen = this->addWidget(&genGUI);
    int idx = this->addWidget(&portGUI);
    setCurrentIndex(idx);


    //TODO: remove leak
    /*QSignalMapper *mapper = new QSignalMapper(this);
    mapper->setMapping(ui->pushButton, idx);
    QObject::connect(ui->pushButton, SIGNAL(clicked()), mapper, SLOT(map()));
    QObject::connect(mapper, SIGNAL(mapped(int)), this, SLOT(setCurrentIndex(int)));*/


    //QObject::connect( ui->pushButton, SIGNAL(clicked()), this, SLOT(updateIcons(int)) );
}

SwitchWindow::~SwitchWindow()
{
    delete ui;
}
