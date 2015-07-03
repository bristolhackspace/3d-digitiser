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
//angles when the arm is touched down in centre
float encoder_init_angle[3]={45,32.043,-113.174};

//these are angles
double A; //end pivot = encoder 0
double B; //mid pivot = encoder 1
double C; //base pivot = encoder 2

//steps per encoder
#define STEPS 2048.0

//length of both arm sections
#define ARM 176.777

//height of first pivot
#define H0 (80.782-1.2)

void setup()
{
  setup_usdigi_encoder();
  //store steps of encoders at startup (as they are absolute)
  encoder_init_step[0]=get_pos(0);
  encoder_init_step[1]=get_pos(1);
  encoder_init_step[2]=get_pos(2);
  
  Serial.begin(19200);
}

/* 
returns encoder's angle
compares current steps to starting steps & angle
*/
double get_angle(int encoder_num)
{
    int pos = get_pos(encoder_num);
    int diff = encoder_init_step[encoder_num] - pos;
    double angle = encoder_init_angle[encoder_num] + diff * (2 * M_PI / STEPS);
    return angle;
}

void loop()
{
    double B=get_angle(0); //end
    double A=get_angle(1); //mid
    double C=get_angle(2); //base

    double r=cos(A)*ARM+cos(A+B)*ARM;
    double h=sin(A)*ARM+sin(A+B)*ARM+H0;

    //don't know what the 125 is
    double y=r*cos(C)-125;
    double x=r*sin(C)-125;
    double z=h;

    /* put this back in when it's working
    sendFloat(x,'x');
    sendFloat(y,'y');
    sendFloat(z,'z');
    */

    //print ABC angles for debugging
    char buf[25];
    sprintf(buf, "%6.0lf,%6.0lf,%6.0lf", A,B,C);
    Serial.println(buf);
    delay(100);
}
