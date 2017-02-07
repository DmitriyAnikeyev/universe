#ifndef QGRSCENE_H
#define QGRSCENE_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QEvent>
#include <QDebug>

class qgrScene : public QGraphicsView
{
    Q_OBJECT
public:
    explicit qgrScene(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *ev);

    int x,y;

signals:
    void Mouse_Pressed();
};

#endif // QGRSCENE_H
