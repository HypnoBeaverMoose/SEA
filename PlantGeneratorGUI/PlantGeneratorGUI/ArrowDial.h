#ifndef ARROWDIAL_H
#define ARROWDIAL_H

#include <QDial>
#include <qlabel.h>
#include <QPixmap>

class ArrowDial : public QDial
{
    Q_OBJECT

    public:
        struct Area
        {
            int id;
            float left;
            float right;
        };

        ArrowDial(QWidget *parent = 0);

        Area getCurArea();

    protected:
        void paintEvent(QPaintEvent *);

    private:
        QPixmap arrowImg;
};

#endif // ARROWDIAL_H
