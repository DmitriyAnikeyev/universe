#ifndef CLASSES_H
#define CLASSES_H

#include <vector>
#include <fstream>
#include <cmath>

/*
 * *************
 *    DEFINE
 * *************
 */

#define  FIELD_SIZE1 25
#define  FIELD_SIZE2 25
#define ELEMENTS_AMOUNT 10
#define DIFFUSION 0.1
#define SIZE 10
#define CONVERT 0.3 //Для термоядерной реакции
#define CONVERT2 0.1 //Для распада
#define CELL_SIZE 16 //Для прорисовки

#define EPS 0.001
#define COLOR_CONST 2
#define ENERGY_CONST 1000000


/*
 *   Массы элементов
 */

#define mH 1
#define mHe 4
#define mLi 7
#define mBe 9
#define mB 11
#define mC 12
#define mN 14
#define mO 16
#define mF 19
#define mNe 20

/*
 * Константы, изменяемые прямо из диалогового окна
 */

double GRAVITY = 20;
double TIME = 2;
double SIGMA = 0.5; //Используется при расчете вероятности термояда.
int MASS = 100; // Задает диапазон начальных рандомизировнных масс: от 0 до MASS
int SPEED = 10; // Задает диапазон начальных рандомизированных скоростей: от -SPEED/2 до SPEED/2
double BR_CONST = 3000; //Определяет яркость цветов
int colorset = 0;/* Цветовая гамма:
                  * 0 - стандартная
                  * 1 - по массе
                  * 2 - по температуре
                  */


std::vector<double> ratio_of_massses(ELEMENTS_AMOUNT);
/*
 * Отношения масс при вводе их руками из диалогового окна
 * Кнопка ОК запускает инициализацию.
 */

int check[ELEMENTS_AMOUNT];//учитывание элементов при прорисовке
bool check_star;

float convert[ELEMENTS_AMOUNT] = {0.2,0.1,0.1,0.08,0.07,0.06,0.05,0.02,0.01,0};

struct color //Структура для компонентов цвета клетки
{
    int r;
    int g;
    int b;
    int br;
};



struct comp //состав клетки
{
    float element[ELEMENTS_AMOUNT];
};

class intvector
{
    public:
    int x;
    int y;
};

class floatvector
{
public:
    float x;
    float y;
    float abs();
    float abs3();
};

float floatvector::abs()
{
    return sqrt(x*x+y*y);
}

float floatvector::abs3()
{
    return abs()*abs()*abs();
}


/*
 *
 *  Класс отвечающий за газ в клетке.
 *  В него входят моли элементов и их скорости.
 *
 */
class gas
{
public:
comp composition;
floatvector speed[ELEMENTS_AMOUNT];
};

int weight[ELEMENTS_AMOUNT]={mH,mHe,mLi,mBe,mB,mC,mN,mO,mF,mNe}; //Массив масс элементов

/*
 *
 * Массив задающий цвета элементов
 *
 */

color colors[ELEMENTS_AMOUNT]={{0,0,200, 255},
                               {200,200,0,255},
                               {150,150,150,255},
                               {220,220,220,255},
                               {90,10,10,255},
                               {10,10,10, 255},
                               {92,250,242,255},
                               {118,255,122,255},
                               {255,233,16,255},
                               {51,204,255,255}};


/***************************************************************
 *
 * *******************   STARS    ******************************
 *
 ***************************************************************
 */

class star
{
public:
    star();
    float mass; //масса
    comp composition;// состав
    floatvector speed;//скорость
    void step();
};

star::star() //иницаилизируем нулями
{
    speed.x=0;
    speed.y=0;
    int i;
    for(i=0; i<ELEMENTS_AMOUNT; i++)
    composition.element[i]=0;
    mass = 0;
}

/***************************************************************
 *
 * *******************   CELL     ******************************
 *
 ***************************************************************
 */

class cell
{
public:
    intvector coord_; //координаты клетки
    float mass_;      //суммарная масса газа и звезд
    gas gase_;        //газ в данной клетке
    color c;          //распределение цветов для данной клетки
    std::vector<star> stars; // массив звезд
    int amount_of_stars_;
    float count_mass(gas gase); //функция подсчета массы
    void set_color(gas gase);   //функция подсчета цвета на основе цветов элементов
    void init(intvector coord, gas gase);
    void set_color_mass(gas gase); //функция подсчета цвета на основе массы клетки
    void set_color_temperature(gas gase); //функция подсчета цвета на основе энергии клетки
};

float cell::count_mass(gas gase)
{
    int i;
    float mass = 0;
    for (i=0; i<ELEMENTS_AMOUNT; i++)
    {
        mass+=(gase.composition.element[i]*weight[i]);
    }

    return mass;
}

void cell::set_color(gas gase)
{
    int i,r,g,b;
    r=0;
    g=0;
    b=0;

    for (i=0;i<ELEMENTS_AMOUNT; i++)
    {
        r+=gase.composition.element[i]*colors[i].r*weight[i]*check[i];
        g+=gase.composition.element[i]*colors[i].g*weight[i]*check[i];
        b+=gase.composition.element[i]*colors[i].b*weight[i]*check[i];
    }

    float mass = 0;
    mass = count_mass(gase);
    int mass_star = 3000;
    r+=atan(amount_of_stars_/2)*2/M_PI*255*mass_star*check_star;
    g+=atan(amount_of_stars_/2)*2/M_PI*255*mass_star*check_star;

    if(mass!=0)
    {
        r= (int)r/(mass+mass_star*check_star)*atan(mass/BR_CONST)*2/M_PI;
        g= (int)g/(mass+mass_star*check_star)*atan(mass/BR_CONST)*2/M_PI/1.5;
        b= (int)b/(mass+mass_star*check_star)*atan(mass/BR_CONST)*2/M_PI;
        c = {r,g,b,255};
    }
    else
        c = {0,0,0,0};
 }

void cell::set_color_mass(gas gase)
{
    float mass = 0;
    for (int i=0; i<ELEMENTS_AMOUNT; i++)
    {
        mass+=(gase.composition.element[i]*weight[i]);
    }

    if(check_star)
        for(int k=0;k<amount_of_stars_;k++)
            mass+=stars[k].mass;
    int g;
    g=0;

    g = 255*atan(mass/BR_CONST)*2/M_PI;
    if(g>255)
        g/=2;
    if (mass!=0)
        c={g,g/2,g/7, 255};
    else
        c={0,0,0,0};

}

void cell::set_color_temperature(gas gase)
{
    float energy = 0;
    for(int k =0; k<ELEMENTS_AMOUNT; k++)
    {
        energy+=gase.speed[k].abs()*gase.speed[k].abs()*gase.composition.element[k]*weight[k]*check[k];
    }

    int g;
    g=0;

    g = 255*atan(energy/BR_CONST)*2/M_PI;
    if(g>255)
        g/=2;
    if (energy!=0)
        c={g,g/2,g/7, 255};
    else
        c={0,0,0,0};

}
void cell::init(intvector coord, gas gase)
{
    coord_ = coord;
    amount_of_stars_ = 0;
    gase_ = gase;
    mass_ = count_mass(gase);
    set_color(gase_);
}


/***************************************************************
 *
 * *******************   FIELD    ******************************
 *
 ***************************************************************
 */

class field
{
public:
    field();
    cell cells[FIELD_SIZE1][FIELD_SIZE2]; //клетки поля
    void init(const char * filename); //инициализация из файла
    void export_to(const char *filename);//экспорт поля в файл
    void step(); //функция преобразования поля
    void randomize(); //функция инициализации поля с произвольными массами и скоростями
    void init_by_hand();//функция инициализации поля с произвольными скоростями и заданным соотношением на моли элементов
    void set_color();   //пересчитывает цвет всех клеток поля  в зависимости от выбранного режима
};


field::field()
{
    intvector coord;
    gas gase;

    int k;
    for(k=0;k<ELEMENTS_AMOUNT;k++)
    {
        gase.speed[k].x=0;
        gase.speed[k].y=0;
        gase.composition.element[k] = 0;
    }
    int i,j;
    for(i=0;i<FIELD_SIZE1;i++)
        for(j=0;j<FIELD_SIZE2;j++)
        {
            coord.x = i+1;
            coord.y = j+1;
            cells[i][j].init(coord, gase);
        }
}


void field::init(const char *filename)
{
    std::ifstream inFile;
    inFile.open(filename);
    intvector coord;
    gas gase;
    std::string temp;
    int flag = 1;

    int i,j,k;
    for (i=0; i<FIELD_SIZE1; i++)
    {
        if(!flag)
            break;
        for (j=0; j<FIELD_SIZE2; j++)
        {
            for(k=0; k<3*ELEMENTS_AMOUNT; k++)
            {
                if(k==0)
                {
                    if(!(inFile >> coord.x))
                        flag = 0;//для комментов и номеров клеток
                    inFile >> coord.y;
                }

                if(k<ELEMENTS_AMOUNT)
                    inFile >> gase.composition.element[k]; //первая строка - массы

                if((k>=ELEMENTS_AMOUNT)&&(k<2*ELEMENTS_AMOUNT))
                    inFile >> gase.speed[k%ELEMENTS_AMOUNT].x;// вторая - координата скорости по x

                if((k>=2*ELEMENTS_AMOUNT)&&(k<3*ELEMENTS_AMOUNT))
                    inFile >> gase.speed[k%ELEMENTS_AMOUNT].y; // третья - скорость по y
            }
            if (flag)
                cells[i][j].init(coord,gase);
            else break;
        }
    }
}


void field::init_by_hand()
{
    using namespace std;
    intvector coord;
    gas gase;
    srand(time(0));

    double summ = 0;
    for(int l=0; l<ELEMENTS_AMOUNT; l++)
    {
        summ+=ratio_of_massses[l];
    }

    int i,j,k;
    for (i=0; i<FIELD_SIZE1; i++)
    {
        for (j=0; j<FIELD_SIZE2; j++)
        {
            coord.x = i+1;
            coord.y = j+1;
            for(k=0; k<3*ELEMENTS_AMOUNT; k++)
            {

                if(k<ELEMENTS_AMOUNT)
                    gase.composition.element[k]= (rand() % MASS+1) * (ratio_of_massses[k]+EPS)/summ; //первая строка - массы

                if((k>=ELEMENTS_AMOUNT)&&(k<2*ELEMENTS_AMOUNT))
                    gase.speed[k%ELEMENTS_AMOUNT].x = rand() % SPEED-SPEED/2;// вторая - координата скорости по x

                if((k>=2*ELEMENTS_AMOUNT)&&(k<3*ELEMENTS_AMOUNT))
                    gase.speed[k%ELEMENTS_AMOUNT].y=rand() % SPEED-SPEED/2; // третья - скорость по y
            }
            cells[i][j].init(coord,gase);
        }
    }
}

void field::randomize()
{
    using namespace std;
    intvector coord;
    gas gase;
    srand(time(0));

    int i,j,k;
    for (i=0; i<FIELD_SIZE1; i++)
    {
        for (j=0; j<FIELD_SIZE2; j++)
        {
            coord.x = i+1;
            coord.y = j+1;
            for(k=0; k<3*ELEMENTS_AMOUNT; k++)
            {

                if(k<ELEMENTS_AMOUNT)
                    gase.composition.element[k]=rand() % MASS; //первая строка - массы

                if((k>=ELEMENTS_AMOUNT)&&(k<2*ELEMENTS_AMOUNT))
                    gase.speed[k%ELEMENTS_AMOUNT].x =rand() % SPEED-SPEED/2;// вторая - координата скорости по x

                if((k>=2*ELEMENTS_AMOUNT)&&(k<3*ELEMENTS_AMOUNT))
                    gase.speed[k%ELEMENTS_AMOUNT].y=rand() % SPEED-SPEED/2; // третья - скорость по y
            }
            cells[i][j].init(coord,gase);
        }
    }
}


void field::export_to(const char * filename)
{
    int i,j,k;
    std::ofstream outFile;
    outFile.open(filename);
    gas gase;

    for (i=0; i<FIELD_SIZE1; i++)
    {
        for (j=0; j<FIELD_SIZE2; j++)
        {
            gase = cells[i][j].gase_;
            for(k=0; k<3*ELEMENTS_AMOUNT; k++)
            {
                if (k==0)
                    outFile<<i+1<<" "<<j+1<<std::endl;
                if(k<ELEMENTS_AMOUNT)
                    outFile << gase.composition.element[k]<<" "; //первая строка - массы
                if (k==ELEMENTS_AMOUNT)
                    outFile<<std::endl;
                if((k>=ELEMENTS_AMOUNT)&&(k<2*ELEMENTS_AMOUNT))
                    outFile << gase.speed[k%ELEMENTS_AMOUNT].x<<" ";// вторая - координата скорости по x
                if(k==2*ELEMENTS_AMOUNT)
                    outFile<<std::endl;
                if((k>=2*ELEMENTS_AMOUNT)&&(k<3*ELEMENTS_AMOUNT))
                    outFile << gase.speed[k%ELEMENTS_AMOUNT].y<<" "; // третья - скорость по y
            }
            outFile<<std::endl<<std::endl;
        }
    }
};



void field::step()
{
    int i,j,k,l,t;
    gas zero_gase;
    for(t=0; t< ELEMENTS_AMOUNT; t++)
    {
        zero_gase.composition.element[t]=0;
        zero_gase.speed[t].x=zero_gase.speed[t].y=0;
    }

    floatvector impulsevector;
    floatvector othersimpulsevector;


    //Распад газа
    for(i=0; i<FIELD_SIZE1; i++)
        for(j=0; j<FIELD_SIZE2; j++)
            for(t=0;t<ELEMENTS_AMOUNT-1;t++)
            {
                cells[i][j].gase_.composition.element[t]+=cells[i][j].gase_.composition.element[t+1]*CONVERT2*weight[t+1]/weight[t];
                cells[i][j].gase_.composition.element[t+1]-=cells[i][j].gase_.composition.element[t+1]*CONVERT2;
            }

    //Образование звезд
    float starmass;
    star newstar;
        for(i=0; i<FIELD_SIZE1; i++)
            for(j=0; j<FIELD_SIZE2; j++)
            {
                for(l=0;l<3;l++)
                {
                    starmass=0;
                    for(k=0;k<cells[i][j].amount_of_stars_; k++)
                        starmass+=cells[i][j].stars[k].mass;

                    if((rand()%75+cells[i][j].mass_/500)>(75+100*starmass/(cells[i][j].mass_+EPS)))//создание новой звезды
                    {
                        newstar.mass=0;
                        newstar.speed.x=newstar.speed.y=0;

                        for(t=0;t<ELEMENTS_AMOUNT;t++)
                        {
                            newstar.composition.element[t]=(1+rand()%20)*cells[i][j].gase_.composition.element[t]/100;
                            cells[i][j].gase_.composition.element[t]-=newstar.composition.element[t];
                            newstar.speed.x+=cells[i][j].gase_.speed[t].x*weight[t]*newstar.composition.element[t];//импульс
                            newstar.speed.y+=cells[i][j].gase_.speed[t].y*weight[t]*newstar.composition.element[t];//импульс
                            newstar.mass+=weight[t]*newstar.composition.element[t];
                        }

                        newstar.speed.x=newstar.speed.x/(EPS+newstar.mass);
                        newstar.speed.y=newstar.speed.y/(EPS+newstar.mass);
                        cells[i][j].stars.push_back(newstar);
                        cells[i][j].amount_of_stars_++;
                    }
                }

                //Удаление звезд
                for(k=0;k<cells[i][j].amount_of_stars_; k++)
                if (rand()%100>90)
                {

                   for(t=0;t<ELEMENTS_AMOUNT;t++)
                   {
                        cells[i][j].gase_.speed[t].x=(cells[i][j].gase_.speed[t].x*cells[i][j].gase_.composition.element[t]+cells[i][j].stars[k].composition.element[t]*cells[i][j].stars[k].speed.x)
                           /(EPS+cells[i][j].gase_.composition.element[t]+cells[i][j].stars[k].composition.element[t]);//((mu+Mv)/(m+M)
                        cells[i][j].gase_.speed[t].y=(cells[i][j].gase_.speed[t].y*cells[i][j].gase_.composition.element[t]+cells[i][j].stars[k].composition.element[t]*cells[i][j].stars[k].speed.y)
                           /(EPS+cells[i][j].gase_.composition.element[t]+cells[i][j].stars[k].composition.element[t]);//((mu+Mv)/(m+M))

                        cells[i][j].gase_.composition.element[t]+=cells[i][j].stars[k].composition.element[t];
                   }

                   cells[i][j].stars.erase(cells[i][j].stars.begin()+k);
                   cells[i][j].amount_of_stars_--;
                   k--;
                }
            }
    //Сгорание звезд
        for(i=0; i<FIELD_SIZE1; i++)
            for(j=0; j<FIELD_SIZE2; j++)
                for(k=0;k<cells[i][j].amount_of_stars_; k++)
                    for(t=0;t<ELEMENTS_AMOUNT-1;t++)
                    {
                        cells[i][j].stars[k].composition.element[t+1]+=cells[i][j].stars[k].composition.element[t]*convert[t]*weight[t]/weight[t+1];//термояд
                        cells[i][j].stars[k].composition.element[t]-=cells[i][j].stars[k].composition.element[t]*convert[t];
                    }

//Подсчет гравитации
    floatvector Force[FIELD_SIZE1][FIELD_SIZE2];
    floatvector distance;
    for(i=0; i<FIELD_SIZE1; i++)
        for(j=0; j<FIELD_SIZE2; j++)
        {
            Force[i][j].x=Force[i][j].y = 0;
            for(k=0; k<FIELD_SIZE1; k++)
                for(l=0; l<FIELD_SIZE2; l++)
                {
                    distance.x=(k-i)*SIZE;
                    distance.y=(l-j)*SIZE;
                    //MG/R^2*(x/R)
                    if(distance.abs() > 0)
                    {
                        Force[i][j].x += cells[k][l].mass_ * GRAVITY / distance.abs3() * distance.x;
                        Force[i][j].y += cells[k][l].mass_ * GRAVITY / distance.abs3() * distance.y;
                    }
                }
        }


//Подсчет поправки скорости
    for(i=0; i<FIELD_SIZE1; i++)
        for(j=0; j<FIELD_SIZE2; j++)
        {
            impulsevector.x = 0;
            impulsevector.y = 0;

            for(k=0;k<ELEMENTS_AMOUNT;k++)
            {
                //m*U*концентрация
                impulsevector.x += (cells[i][j].gase_.speed[k].x * cells[i][j].gase_.composition.element[k] * weight[k]);
                impulsevector.y += (cells[i][j].gase_.speed[k].y * cells[i][j].gase_.composition.element[k] * weight[k]);
            }

           // std::cout<<i<<" "<<j<<" "<<impulsevector.abs()<<std::endl;

            for(k=0;k<ELEMENTS_AMOUNT;k++)
            {
                //вычисляем импульс остальной системы.
                othersimpulsevector.x = impulsevector.x - (cells[i][j].gase_.speed[k].x * cells[i][j].gase_.composition.element[k] * weight[k]);
                othersimpulsevector.y = impulsevector.y - (cells[i][j].gase_.speed[k].y * cells[i][j].gase_.composition.element[k] * weight[k]);
                cells[i][j].gase_.speed[k].x += othersimpulsevector.x/(cells[i][j].mass_-cells[i][j].gase_.composition.element[k]*weight[k]+EPS)*DIFFUSION;
                cells[i][j].gase_.speed[k].y += othersimpulsevector.y/(cells[i][j].mass_-cells[i][j].gase_.composition.element[k]*weight[k]+EPS)*DIFFUSION;
                cells[i][j].gase_.speed[k].y+=cells[i][j].gase_.speed[k].x*((rand()%50)-25)/100;
                cells[i][j].gase_.speed[k].x+=cells[i][j].gase_.speed[k].y*((rand()%50)-25)/100;
            }
        }



//std::cout<<"//Влияние гравитации"<<std::endl;
    for(t=0; t<ELEMENTS_AMOUNT; t++)
    {
        for(i=0; i<FIELD_SIZE1; i++)
        {
            for(j=0; j<FIELD_SIZE2; j++)
            {
            //Ускорение
                cells[i][j].gase_.speed[t].x +=TIME*Force[i][j].x/(cells[i][j].mass_+EPS);
                cells[i][j].gase_.speed[t].y +=TIME*Force[i][j].y/(cells[i][j].mass_+EPS);
            }
        }
    }



//std::cout<<"//Сдвиг каждой клетки с подсчетом новых параметров."<<std::endl;
    float probability[FIELD_SIZE1][FIELD_SIZE2];
    float statsigma;
    intvector init;
    floatvector point;
//std::cout<<"///Новое поле"<<std::endl;
    cell newcells[FIELD_SIZE1][FIELD_SIZE2];
    for(k=0; k<FIELD_SIZE1; k++)
        for(l=0; l<FIELD_SIZE2; l++)
        {
            init.x=k+1;
            init.y=l+1;
            newcells[k][l].init(init,zero_gase);
        }

    int dx;
    int dy;
    for(t=0; t<ELEMENTS_AMOUNT; t++)
    {
        for(i=0; i<FIELD_SIZE1; i++)
        {
            for(j=0; j<FIELD_SIZE2; j++)
            {
                newcells[i][j].gase_.speed[t].x=0;
                point.x=(int)(i+cells[i][j].gase_.speed[t].x*TIME/SIZE)%FIELD_SIZE1;
                point.y=(int)(j+cells[i][j].gase_.speed[t].y*TIME/SIZE)%FIELD_SIZE2;
                if(point.x<0)
                    point.x+=FIELD_SIZE1;
                if(point.y<0)
                    point.y+=FIELD_SIZE2;
                statsigma=0;

                for(k=0; k<FIELD_SIZE1; k++)
                    for(l=0; l<FIELD_SIZE2; l++)
                    {
                        dx=k-point.x;
                        dy=l-point.y;
                        if(dx>(FIELD_SIZE1-dx))
                           dx=FIELD_SIZE1-dx;
                        if(dy>(FIELD_SIZE2-dy))
                           dy=FIELD_SIZE2-dy;
                        probability[k][l]=exp((-dx*dx-dy*dy)/SIGMA/(cells[i][j].gase_.speed[t].abs()+SPEED*3/10));
                        statsigma+=probability[k][l];
                    }
                for(k=0; k<FIELD_SIZE1; k++)
                    for(l=0; l<FIELD_SIZE2; l++)
                    {

                    //новый элемент
                        newcells[k][l].gase_.composition.element[t]+=cells[i][j].gase_.composition.element[t]*probability[k][l]/(statsigma+EPS);

                    //новая скорость x
                        newcells[k][l].gase_.speed[t].x=(newcells[k][l].gase_.speed[t].x*newcells[k][l].mass_+
                        (cells[i][j].gase_.composition.element[t]*probability[k][l]/statsigma)*cells[i][j].gase_.speed[t].x*weight[t])/

                    //СЛОЖНА!!
                       (EPS+newcells[k][l].mass_ + cells[i][j].gase_.composition.element[t]*probability[k][l]/(statsigma+EPS)*weight[t]);

                    //новая скорость y
                        newcells[k][l].gase_.speed[t].y=(newcells[k][l].gase_.speed[t].y*newcells[k][l].mass_+
                        (cells[i][j].gase_.composition.element[t]*probability[k][l]/statsigma)*cells[i][j].gase_.speed[t].y*weight[t])/
                    //СЛОЖНА!!
                       (EPS+newcells[k][l].mass_ +  cells[i][j].gase_.composition.element[t]*probability[k][l]/(statsigma+EPS)*weight[t]);

                    //новая масса
                        newcells[k][l].mass_+=cells[i][j].gase_.composition.element[t]*probability[k][l]/(statsigma+EPS)*weight[t];
                    }
            }
        }
    }


//std::cout<<"//движение звезд"<<std::endl;

        intvector newstarpoint;
        for(i=0; i<FIELD_SIZE1; i++)
        {
            for(j=0; j<FIELD_SIZE2; j++)
            {
                for(t=cells[i][j].amount_of_stars_-1;t>=0;t--)
                 {

                     cells[i][j].stars[t].speed.x +=TIME*Force[i][j].x/(cells[i][j].mass_+EPS);
                     cells[i][j].stars[t].speed.y +=TIME*Force[i][j].y/(cells[i][j].mass_+EPS);
                    newstarpoint.x=(int)(i+(int)cells[i][j].stars[t].speed.x*TIME)%FIELD_SIZE1;
                    newstarpoint.y=(int)(j+(int)cells[i][j].stars[t].speed.y*TIME)%FIELD_SIZE2;

                    if(newstarpoint.x<0)
                        newstarpoint.x+=FIELD_SIZE1;
                    if(newstarpoint.y<0)
                        newstarpoint.y+=FIELD_SIZE2;
                    newcells[newstarpoint.x][newstarpoint.y].stars.push_back(cells[i][j].stars[t]);//вот тут seg fault
                    newcells[newstarpoint.x][newstarpoint.y].amount_of_stars_++;
                    cells[i][j].stars.pop_back();
                 }
            }
        }

        for(i=0; i<FIELD_SIZE1; i++)
        {
            for(j=0; j<FIELD_SIZE2; j++)
            {
            cells[i][j]=newcells[i][j];
            }
        }

    for(i=0; i<FIELD_SIZE1; i++)
        for(j=0; j<FIELD_SIZE2; j++)
        {
            cells[i][j].mass_=0;
            for(k=0;k<ELEMENTS_AMOUNT;k++)
        //+звезды?
            cells[i][j].mass_+=cells[i][j].gase_.composition.element[k]*weight[k];
            for(k=0;k<cells[i][j].amount_of_stars_;k++)
                   cells[i][j].mass_+=cells[i][j].stars[k].mass;
        }

    set_color();
}

void field::set_color()
{
    for(int i=0; i<FIELD_SIZE1;i++)
        for(int j=0;j<FIELD_SIZE2; j++)
        {
           switch(colorset)
           {
                case 0:
                    cells[i][j].set_color(cells[i][j].gase_);
                    break;
                case 1:
                    cells[i][j].set_color_mass(cells[i][j].gase_);
                    break;
                case 2:
                    cells[i][j].set_color_temperature(cells[i][j].gase_);
                    break;
            }
        }
}

#endif // CLASSES_H
