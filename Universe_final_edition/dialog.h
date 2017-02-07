#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QColor>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QTime>
#include "mythread.h"

//#include "classes.h"

class field;

namespace Ui {
    class Dialog;
}


class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    friend class field;

public slots:
    void print_field();
    void onMouse_Pressed();
    void onstep(double t);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_doubleSpinBox_2_valueChanged(double arg1);

    void on_doubleSpinBox_3_valueChanged(double arg1);

    void on_doubleSpinBox_4_editingFinished();

    void on_doubleSpinBox_5_valueChanged(double arg1);

    void on_doubleSpinBox_4_valueChanged(double arg1);

    void on_doubleSpinBox_6_valueChanged(double arg1);

    void on_doubleSpinBox_7_valueChanged(double arg1);

    void on_doubleSpinBox_8_valueChanged(double arg1);

    void on_doubleSpinBox_9_valueChanged(double arg1);

    void on_doubleSpinBox_10_valueChanged(double arg1);

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_doubleSpinBox_11_valueChanged(double arg1);

    void on_pushButton_9_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_10_clicked();

    void on_doubleSpinBox_12_valueChanged(double arg1);

    void on_doubleSpinBox_13_valueChanged(double arg1);

    void on_doubleSpinBox_14_valueChanged(double arg1);

    void on_doubleSpinBox_15_valueChanged(double arg1);

    void on_doubleSpinBox_16_valueChanged(double arg1);

    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_checkBox_3_stateChanged(int arg1);

    void on_checkBox_4_stateChanged(int arg1);

    void on_checkBox_5_stateChanged(int arg1);

    void on_checkBox_6_stateChanged(int arg1);

    void on_checkBox_7_stateChanged(int arg1);

    void on_checkBox_8_stateChanged(int arg1);

    void on_checkBox_9_stateChanged(int arg1);

    void on_checkBox_10_stateChanged(int arg1);

    void on_checkBox_11_stateChanged(int arg1);

public:
    Ui::Dialog *ui;
    QGraphicsScene *scene;
    QGraphicsEllipseItem *ellipse;
    QGraphicsRectItem *rectangle, **rect_mass;
    //void print_field(field B);
    MyThread* mthread;
};

#endif // DIALOG_H
