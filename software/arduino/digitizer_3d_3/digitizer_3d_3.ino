void sendFloat(float f,unsigned t) {
  byte * b = (byte *) &f;
  Serial.write(t);
  Serial.write(b[0]);
  Serial.write(b[1]);
  Serial.write(b[2]);
  Serial.write(b[3]);
}

volatile long encoder[3]={0,0,0};							//-Encoder value

double A; //end pivot = encoder 0
double B; //mid pivot = encoder 1
double C; //base pivot = encoder 2

//steps per encoder
#define STEPS 2048.0

void setup()
{

  //setup encoders for when the arm is touched down
  encoder[0]=(45.0/360)*STEPS; //45deg //base
  encoder[1]=(32.043/360.0)*STEPS; //mid
  encoder[2]=(-113.174/360.0)*STEPS; //end pivot
  setup_usdigi_encoder();
  
  Serial.begin(19200);
}

//length of both arm sections
#define ARM 176.777

//height of first pivot
#define H0 (80.782-1.2)

void loop()
{
    double A=(double)encoder[1]*M_PI/(2*STEPS); //mid
    double B=(double)encoder[2]*M_PI/(2*STEPS); //end
    double C=(double)encoder[0]*M_PI/(2*STEPS); //base

    double r=cos(A)*ARM+cos(A+B)*ARM;
    double h=sin(A)*ARM+sin(A+B)*ARM+H0;

    double y=r*cos(C)-125;
    double x=r*sin(C)-125;
    double z=h;

    /*
    sendFloat(x,'x');
    sendFloat(y,'y');
    sendFloat(z,'z');
    */

    char buf[25];
    sprintf(buf, "%6.0lf,%6.0lf,%6.0lf", A,B,C);
    Serial.println(buf);
    delay(100);
}
