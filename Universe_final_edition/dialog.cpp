#include "dialog.h"
#include "ui_dialog.h"

#include "classes.h"
#include <malloc.h>
#include <stack>
#include <cstdio>
#include <cstdlib>
#include <cstring>

QMutex mutex;
QWaitCondition pause;
std::stack<field *> f;
field* B = new field;
field* C;
std::string out = "/home/nikolay/Universe_last/input4.txt";
std::string out_for_steps[1000];
int num_ofStep = 0;
//bool color_changed = false;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->doubleSpinBox_11->setMaximum(10000);
    ui->doubleSpinBox_12->setMaximum(10000);
    ui->doubleSpinBox_13->setMaximum(10000);
    ui->doubleSpinBox_14->setMaximum(100);
    ui->doubleSpinBox_15->setMaximum(10000);
    ui->doubleSpinBox_16->setMaximum(10000);
    scene = new QGraphicsScene(this);
    scene->setSceneRect(CELL_SIZE,CELL_SIZE,CELL_SIZE*FIELD_SIZE1,CELL_SIZE*FIELD_SIZE2);
    ui->graphicsView->setSceneRect(CELL_SIZE,CELL_SIZE,CELL_SIZE*FIELD_SIZE1,CELL_SIZE*FIELD_SIZE2);
    ui->graphicsView->x =1;
    ui->graphicsView->y = 1;


    for (int k=0;k<10;k++)
        check[k] = 1;

    mthread = new MyThread(this);
    mthread->Stop = false;
    mthread->Pause = false;
    mthread->Random = false;
    mthread->Hand = false;
    mthread->Prev = false;
    mthread->Next = false;
    connect(mthread,SIGNAL(print()),this,SLOT(print_field()));
    connect(ui->graphicsView, SIGNAL(Mouse_Pressed()), this, SLOT(onMouse_Pressed()));

}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::print_field()
{
    ui->graphicsView->setScene(scene);
    QColor* sampleColor = (QColor*) calloc(FIELD_SIZE1*FIELD_SIZE2, sizeof(QColor));
    //RGB: красная, зелёная и синяя компоненты + яркость
    //QColor sampleColor1 = QColor(200, 0, 0, 255);
    QBrush sampleBrush;
    QPen blackpen(Qt::black);
    blackpen.setWidth(1);
    //rectangle = scene->addRect(A.coord_.x*10, A.coord_.y*10, 100, 100, blackpen, whiteBrush);
    rect_mass = (QGraphicsRectItem**) calloc(FIELD_SIZE1*FIELD_SIZE2, sizeof(QGraphicsRectItem*));

    for (int i = 0; i < FIELD_SIZE1; ++i)
        for (int j = 0; j < FIELD_SIZE2; ++j)
        {
            //if (B.cells[i][j].c.br == 0) exit(-1);
            sampleColor[FIELD_SIZE1*i+j] = QColor(B->cells[i][j].c.r, B->cells[i][j].c.g, B->cells[i][j].c.b, B->cells[i][j].c.br);
            sampleBrush = QBrush(sampleColor[FIELD_SIZE1*i+j]);
            rect_mass[FIELD_SIZE1*i+j] = scene->addRect(B->cells[i][j].coord_.x*CELL_SIZE, B->cells[i][j].coord_.y*CELL_SIZE, CELL_SIZE, CELL_SIZE, blackpen, sampleBrush);
        }
   onMouse_Pressed();
}

/*
 *
 *
 * THREAD
 *
 *
 */

MyThread::MyThread(QObject *parent) :
    QThread(parent)
{
}

MyThread::~MyThread()
{

}

void MyThread::run()
{
    QTime t;
    t.start();
    //int j;

    std::string output = "/home/nikolay/Universe_last/for_steps/out";
    B->set_color();
    char temp[4];
    for (int j = 0; j<1000; j++)
    {
        sprintf(temp,"%d",j);
        out_for_steps[j] = output + temp;
    }
    while(1)
    {
        if(this->Stop)
            break;

        if(this->Pause)
        {
           pause.wait(&mutex);

           if(this->Prev)
           {
                this->Prev = false;
                emit print();
                this->msleep(500);
                continue;
           }
           if(this->Next)
           {
               this->Next = false;
               num_ofStep++;
               B->step();
               B->export_to(out_for_steps[num_ofStep].c_str());
               emit print();
               this->msleep(100);
               continue;
           }
        }

        B->export_to(out_for_steps[num_ofStep].c_str());
        num_ofStep++;
        emit print();

        t.restart();
        B->step();
        t.elapsed();

        this->msleep(100);
    }
}

/***************
 ***************
 *   BUTTONS   *
 ***************
 ***************
 */

void Dialog::on_pushButton_clicked()
{
    //Play
    mthread->start();
}

void Dialog::on_pushButton_2_clicked()
{
    //Pause
    mthread->Pause = true;
}

void Dialog::on_pushButton_3_clicked()
{
    //Stop
    if (mthread->Pause)
        pause.wakeAll();
    mthread->Stop = true;
}

void Dialog::on_pushButton_4_clicked()
{
    //Next Step
    mthread->Next = true;
    pause.wakeAll();
}

void Dialog::on_pushButton_5_clicked()
{
    //Prev Step
    num_ofStep--;
    B->init(out_for_steps[num_ofStep].c_str());
    B->set_color();
    mthread->Prev = true;
    pause.wakeAll();
    //print_field(B);
}

void Dialog::on_pushButton_6_clicked()
{
    //Continue
    mthread->Pause = false;
    pause.wakeAll();
}

void Dialog::on_pushButton_7_clicked()
{
    //Randomize
    B->randomize();
    B->set_color();
}

void Dialog::on_pushButton_8_clicked()
{
    //OK
    B->init_by_hand();
    B->set_color();
}

void Dialog::on_pushButton_9_clicked()
{
    //colored
    colorset=0;
    B->set_color();
    print_field();
}

void Dialog::on_pushButton_10_clicked()
{
    //massed
    colorset = 1;
    B->set_color();
    print_field();
}

void Dialog::on_pushButton_11_clicked()
{
    //temperature
    colorset = 2;
    B->set_color();
    print_field();
}


void Dialog::onMouse_Pressed() //Клик мышки
{
    int i = ui->graphicsView->x / CELL_SIZE, j = ui->graphicsView->y / CELL_SIZE;
    ui->infLabel->setText(QString("mH = %1\n mHe = %2\n mLi = %3\n mBe = %4\n mB = %5\n mC = %6\n mN = %7\n mO = %8\n mF = %9\n mNe = %10\n Stars: = %11")
            .arg(B->cells[i][j].gase_.composition.element[0]*weight[0])
            .arg(B->cells[i][j].gase_.composition.element[1]*weight[1])
            .arg(B->cells[i][j].gase_.composition.element[2]*weight[2])
            .arg(B->cells[i][j].gase_.composition.element[3]*weight[3])
            .arg(B->cells[i][j].gase_.composition.element[4]*weight[4])
            .arg(B->cells[i][j].gase_.composition.element[5]*weight[5])
            .arg(B->cells[i][j].gase_.composition.element[6]*weight[6])
            .arg(B->cells[i][j].gase_.composition.element[7]*weight[7])
            .arg(B->cells[i][j].gase_.composition.element[8]*weight[8])
            .arg(B->cells[i][j].gase_.composition.element[9]*weight[9])
            .arg(B->cells[i][j].amount_of_stars_));
    ui->step_label->setText(QString("Step: %1").arg(num_ofStep));
}


/*
 * *******************
 *  DOUBLE SPIN BOXES
 * *******************
 */

void Dialog::on_doubleSpinBox_valueChanged(double arg1)
{
    //H
    ratio_of_massses[0] = arg1;
}

void Dialog::on_doubleSpinBox_2_valueChanged(double arg1)
{
    //He
    ratio_of_massses[1] = arg1;
}

void Dialog::on_doubleSpinBox_3_valueChanged(double arg1)
{
    //Li
    ratio_of_massses[2] = arg1;
}

void Dialog::on_doubleSpinBox_4_valueChanged(double arg1)
{
    //Be
    ratio_of_massses[3] = arg1;
    //j=0;
}

void Dialog::on_doubleSpinBox_5_valueChanged(double arg1)
{
    //B
    ratio_of_massses[4] = arg1;
}

void Dialog::on_doubleSpinBox_6_valueChanged(double arg1)
{
    //C
    ratio_of_massses[5] = arg1;
}

void Dialog::on_doubleSpinBox_7_valueChanged(double arg1)
{
    //N
    ratio_of_massses[6] = arg1;
}

void Dialog::on_doubleSpinBox_8_valueChanged(double arg1)
{
    //O
    ratio_of_massses[7] = arg1;
}

void Dialog::on_doubleSpinBox_9_valueChanged(double arg1)
{
    //F
    ratio_of_massses[8] = arg1;
}

void Dialog::on_doubleSpinBox_10_valueChanged(double arg1)
{
    //Ne
    ratio_of_massses[9] = arg1;
}

void Dialog::on_doubleSpinBox_11_valueChanged(double arg1)
{
    //BR_CONST
    BR_CONST = arg1;
}

void Dialog::on_doubleSpinBox_12_valueChanged(double arg1)
{
    GRAVITY = arg1;
}

void Dialog::on_doubleSpinBox_13_valueChanged(double arg1)
{
    TIME = arg1;
}

void Dialog::on_doubleSpinBox_14_valueChanged(double arg1)
{
    SIGMA = arg1;
}

void Dialog::on_doubleSpinBox_15_valueChanged(double arg1)
{
    MASS = arg1;
}

void Dialog::on_doubleSpinBox_16_valueChanged(double arg1)
{
    SPEED = arg1;
}

void Dialog::on_doubleSpinBox_4_editingFinished()
{
    //
}

void Dialog::onstep(double t)//Слот для дебага
{
    ui->doubleSpinBox->setValue(t);
}

void Dialog::on_checkBox_stateChanged(int arg1)
{
    if (ui->checkBox->isChecked())
        check[0]=1;
    else
        check[0]=0;
    B->set_color();
}

void Dialog::on_checkBox_2_stateChanged(int arg1)
{
    if (ui->checkBox_2->isChecked())
        check[1]=1;
    else
        check[1]=0;
    B->set_color();
}

void Dialog::on_checkBox_3_stateChanged(int arg1)
{
    if (ui->checkBox_3->isChecked())
        check[2]=1;
    else
        check[2]=0;
    B->set_color();
}

void Dialog::on_checkBox_4_stateChanged(int arg1)
{
    if (ui->checkBox_4->isChecked())
        check[3]=1;
    else
        check[3]=0;
    B->set_color();
}

void Dialog::on_checkBox_5_stateChanged(int arg1)
{
    if (ui->checkBox_5->isChecked())
        check[4]=1;
    else
        check[4]=0;
    B->set_color();
}

void Dialog::on_checkBox_6_stateChanged(int arg1)
{
    if (ui->checkBox_6->isChecked())
        check[5]=1;
    else
        check[5]=0;
    B->set_color();
}

void Dialog::on_checkBox_7_stateChanged(int arg1)
{
    if (ui->checkBox_7->isChecked())
        check[6]=1;
    else
        check[6]=0;
    B->set_color();
}

void Dialog::on_checkBox_8_stateChanged(int arg1)
{
    if (ui->checkBox_8->isChecked())
        check[7]=1;
    else
        check[7]=0;
    B->set_color();
}

void Dialog::on_checkBox_9_stateChanged(int arg1)
{
    if (ui->checkBox_9->isChecked())
        check[8]=1;
    else
        check[8]=0;
    B->set_color();
}

void Dialog::on_checkBox_10_stateChanged(int arg1)
{
    if (ui->checkBox_10->isChecked())
        check[9]=1;
    else
        check[9]=0;
    B->set_color();
}

void Dialog::on_checkBox_11_stateChanged(int arg1)
{
    if(ui->checkBox_11->isChecked())
        check_star=1;
    else
        check_star=0;
    B->set_color();
}
