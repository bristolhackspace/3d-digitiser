#include "a2abs.h"

void sendFloat(float f,unsigned t) {
  byte * b = (byte *) &f;
  Serial.write(t);
  Serial.write(b[0]);
  Serial.write(b[1]);
  Serial.write(b[2]);
  Serial.write(b[3]);
}

//this could be done by telling the encoders where they are is 0, but
//I wanted to avoid more usdigital routines
//array of encoder steps
long encoder_init_step[3]={0,0,0};
/*

//angles in RADIANS when the arm is touched down in centre
//http://es.mathworks.com/help/fuzzy/examples/modeling-inverse-kinematics-in-a-robotic-arm.html

If arm was straight horizontal, both joint 0 and joint 1 would be 0. 
A positive lift in either joint will increase the angle

    //0 end
    //1 mid
    //2 base
*/
float encoder_init_angle[3]={M_PI + 1.21,0.516,M_PI/4};

//these are angles
double A; //end pivot = encoder 0
double B; //mid pivot = encoder 1
double C; //base pivot = encoder 2

//steps per encoder
#define STEPS 2048.0

//length of both arm sections
#define ARM 176.777

//height of first pivot
#define H0 87

//base width
#define BW 250

void setup()
{
  setup_a2();
  //store steps of encoders at startup (as they are absolute)
  encoder_init_step[0]=get_pos(0);
  encoder_init_step[1]=get_pos(1);
  encoder_init_step[2]=get_pos(2);
  
  Serial.begin(115200);
}

/* 
returns encoder's angle
compares current steps to starting steps & angle
*/
double get_angle(int encoder_num)
{
    int pos = get_pos(encoder_num);
    int diff = encoder_init_step[encoder_num] - pos;
    double angle = encoder_init_angle[encoder_num] - diff * (2 * M_PI / STEPS);
    return angle;
}

void loop()
{
    if(Serial.available())
    {
        if(Serial.read() == 'a')
        {

            double A=get_angle(0); //end
            double B=get_angle(1); //mid
            double C=get_angle(2); //base

            // kinematics: http://es.mathworks.com/help/fuzzy/examples/modeling-inverse-kinematics-in-a-robotic-arm.html
            double r=cos(B)*ARM+cos(A+B)*ARM;
            double h=sin(B)*ARM+sin(A+B)*ARM+H0;

            double y = BW/2 - r*cos(C);
            double x = BW/2 - r*sin(C);
            double z = h;

            /*
            sendFloat(A,'a');
            sendFloat(B,'b');
            sendFloat(C,'c');
            */
            sendFloat(x,'x');
            sendFloat(y,'y');
            sendFloat(z,'z');
        }
        else
        {
            Serial.flush();
        }
    }
}
