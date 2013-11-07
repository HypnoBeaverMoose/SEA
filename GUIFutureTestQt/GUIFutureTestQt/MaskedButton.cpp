
#include <iostream>
#include <qpixmap.h>
#include <qpainter.h>

#include "MaskedButton.h"

MaskedButton::MaskedButton(QWidget *parent) :
    QPushButton(parent), upImg(), downImg(), btnSize(0, 0)
{
	this->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
	this->setMinimumSize( btnSize );
	this->setMaximumSize( btnSize );

	QObject::connect( this, SIGNAL(clicked()), this, SLOT(clickedSlot()) );
}


void MaskedButton::clickedSlot()
{
	std::cout << "clickedSlot()" << std::endl;
}


QSize MaskedButton::sizeHint() const
{
	return QSize(200, 200);
}


QSize MaskedButton::getSize() const
{
	return btnSize;
}


void MaskedButton::setSize( QSize newSize )
{
	btnSize = newSize;
	this->setMinimumSize( btnSize );
	this->setMaximumSize( btnSize );

	repaint();
}


void MaskedButton::setImages( QImage *up, QImage *down )
{
	upImg   = QPixmap::fromImage(*up);
	downImg = QPixmap::fromImage(*down);
	btnSize = upImg.size();

	repaint();
}


void MaskedButton::paintEvent(QPaintEvent *e)
{
	if ( upImg.isNull() || downImg.isNull() )
		return;

	QPainter p(this);
	if ( !this->isDown() )
		p.drawPixmap( 0, 0, upImg.scaled(btnSize) );
	else p.drawPixmap( 0, 0, downImg.scaled(btnSize) );
}


bool MaskedButton::hitButton( const QPoint &pos ) const
{
	//TODO: add masking code here...

	std::cout << "button hit" << std::endl;

	return true;
}
