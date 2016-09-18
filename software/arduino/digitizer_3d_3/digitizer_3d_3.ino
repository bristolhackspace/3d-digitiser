#include "a2abs.h"

//stupidly wasted 2 other gpios by connecting them to serial rx&tx
#define GPIO1 0
#define LED 2
#define GPIO3 3
#define BUTTON 5

//this could be done by telling the encoders where they are is 0, but
//I wanted to avoid more usdigital routines
//array of encoder steps
long encoder_init_step[3]={0,0,0};
//what the encoders are at startup (arm touching centre point)
float encoder_init_angle[3]={0,0,0};

//these are angles
double A; //end pivot = encoder 0
double B; //mid pivot = encoder 1
double C; //base pivot = encoder 2

//steps per encoder
#define STEPS 2048.0

//length of both arm sections
#define ARM 176.8 // from cad

//height of first pivot from top of base to centre of encoder 1 pivot
#define H0 87.0 // measured
// distance from centre of base to first pivot centre - from cad
#define R0 180.0 //from cad
//base width
#define BW 250

double get_angle(int encoder_num);
void sendFloat(float f,unsigned t);
void init_angles();
float cosine_law(float a, float b, float c);

void setup()
{
  setup_a2();
  //init angles
  init_angles();

  //store steps of encoders at startup (as they are absolute)
  encoder_init_step[0]=get_pos(0);
  encoder_init_step[1]=get_pos(1);
  encoder_init_step[2]=get_pos(2);

  pinMode(BUTTON, INPUT);
  digitalWrite(BUTTON, HIGH);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  
  Serial.begin(115200);
}


void loop()
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

    if(Serial.available())
    {
        if(Serial.read() == 'a')
        {
            digitalWrite(LED, HIGH);

            /*
            sendFloat(A,'a');
            sendFloat(B,'b');
            sendFloat(C,'c');
            */

            sendFloat(x,'x');
            sendFloat(y,'y');
            sendFloat(z,'z');
            sendFloat(!digitalRead(BUTTON),'B');
            digitalWrite(LED, LOW);
        }
        Serial.flush();
    }
}

//send an identifier & a float via serial
void sendFloat(float f,unsigned t) {
  byte * b = (byte *) &f;
  Serial.write(t);
  Serial.write(b[0]);
  Serial.write(b[1]);
  Serial.write(b[2]);
  Serial.write(b[3]);
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

/*
inverse kinematics - only do at startup
calculate all angles in RADIANS when the arm is touched down in centre

If arm was straight horizontal, both joint 0 and joint 1 would be 0. 

A positive lift in either joint will increase the angle

encoder_init_angles has 3 floats:
    0 end
    1 mid
    2 base
*/
void init_angles()
{
    // hypotenuse needed to calc 3rd side of triangle
    float hyp = sqrt(H0*H0 + R0*R0);

    // end angle
    float C = cosine_law(ARM, ARM, hyp);
    encoder_init_angle[1]=M_PI + C;

    // mid angle
    C = cosine_law(ARM, hyp, ARM);
    float gamma = atan(H0/R0);
    encoder_init_angle[0] = C - gamma;

    // base angle
    encoder_init_angle[2] = M_PI / 4;
}

float cosine_law(float a, float b, float c)
{
   return acos((a*a + b*b - c*c) / (2*a*b));
}
