
#include <iostream>
#include <QPainter>

#include "ArrowDial.h"


ArrowDial::ArrowDial( QWidget *parent )
    : QDial(parent), arrowImg()
{
    std::cout << "ArrowDial created." << std::endl;

    QImage img(":/PlantGen/dialArrow.png");
    if ( img.isNull() )
        std::cout << "Warning: unable to load :/PlantGen/dialArrow.png" << std::endl;

    arrowImg = QPixmap::fromImage(img);
}


ArrowDial::Area ArrowDial::getCurArea()
{
    //interpValue = float(value() - minNodge) / (maxNodge - minNodge);
    float interpValue = 0.0f;
    Area a;

    // check which area the pointer is in
    if ( value() >= 0 && value() <= 33 )           // area a => [0 - 33]
    {
        std::cout << "area a" << std::endl;
        a.id        = 1;
        interpValue = float(value()) / 33;
    } else if ( value() >= 34 && value() <= 66 )    // area b => [34 - 66]
    {
        std::cout << "area b" << std::endl;
        a.id        = 2;
        interpValue = float(value() - 34) / 32;
    } else                                          // area c => [67 - 99]
    {
        std::cout << "area c" << std::endl;
        a.id        = 3;
        interpValue = float(value() - 67) / 32;
    }

    a.left  = 1.0f - interpValue;
    a.right = interpValue;

    return a;
}


void ArrowDial::paintEvent(QPaintEvent *)
{
    if ( arrowImg.isNull() )
        return;

    // calculate rotation angle
    int size = this->maximum() - this->minimum() + 1;
    float angle = (360.0f / size) * this->value();

    QPainter p(this);
    p.translate((this->width() / 2) + 26, (this->height() / 2) + 23);
    p.rotate(angle);
    p.translate(-26, -23);
    p.drawPixmap( 0, 0 , arrowImg );
}
