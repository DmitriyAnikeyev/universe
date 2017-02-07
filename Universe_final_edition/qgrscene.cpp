#include "qgrscene.h"

qgrScene::qgrScene(QWidget *parent) :
    QGraphicsView (parent)
{

}

void qgrScene::mousePressEvent(QMouseEvent *ev)
{
    this->x = ev->x();
    this->y = ev->y();
    emit Mouse_Pressed();
}
