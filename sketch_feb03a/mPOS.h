/*
 * mPOS.h
 *
 * Created: 2018/10/10 19:34:55
 *  Author: Vulcan
 */ 


#ifndef MPOS_H_
#define MPOS_H_
double x = 0;//�е�λ��
double y = 0;
double p = 0;

long int x_step = 0;
long int y_step = 0;
long int p_step = 0;


double x1_previous = 0;
double y1_previous = 0;
double p1_previous = 0;

//������Ϣ
static const int INT0A = 19;//INT2
static const int INT0B = 4;
static const int INT1A = 2;//INT4
static const int INT1B = 5;
static const int INT2A = 3;//INT5
static const int INT2B = 6;

//������Ϣ
const double d = 200; //���ּ��160mm
const double d2 = 0;
const double wheel_d = 38;//��ֱ��38mm
const int x_line = 512;
const int y_line = 512;
const int p_line = 512;

//���ڴ������
double x1 = 0;
double y1 = 0;
double p1 = 0;

double x2 = 0;
double y2 = 0;
double p2 = 0;

double _p2;

void blinkX();
void blinkY();
void blinkP();

void POS_begin();
void POS_clear();
void POS_refresh();

#endif /* MPOS_H_ */