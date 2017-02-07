#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QtCore>

class field;
class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = 0);
    ~MyThread();
    void run();
    bool Stop;
    bool Pause;
    bool Random;
    bool Hand;
    bool Prev;
    bool Next;
signals:
    void print();
    void _step(double t);
public slots:

};

#endif // MYTHREAD_H
