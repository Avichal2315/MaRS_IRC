#include <Arduino.h>




//Follow the below mentioned convention
//Make sure if you press A, the wheel goes clockwise with respect to the front of the motor
//When high is to the direction pin, B gets the signal.
//The above point was found out experimentally




//Function declaration




void forward();
void backward();
void spotleft();
void spotright();


void pwm_adjust_for_all_wheels(int);
void call_choice(int);




//Motor Drivers Pin Declaration




//front left
#define dir_fl PC15    
#define pwm_fl PA0




//front right
#define dir_fr PB9
#define pwm_fr PB8




//back left
#define dir_bl PA5
#define pwm_bl PA6




//back right
#define dir_br PB15
#define pwm_br PB14




//choice
int choice = 0;
int prev_choice = 0;
int input_pwm = 0;
int prev_pwm = 0;
int count = 0;








//class declaration of motor
class motor
{
  private:
 
    int dir_pin;
    int pwm_pin;
 
  public:
    motor(int pin1,int pin2);
    void clockwise();
    void anticlockwise();
    void pwm(int input_pwm);
};




motor::motor(int pin1,int pin2)
{
  dir_pin = pin1;
  pwm_pin = pin2;    
  pinMode(dir_pin,OUTPUT);
  pinMode(pwm_pin,OUTPUT);
}




void motor::clockwise() // Viewing the rover form the right side with respect to front of the rover
{
  digitalWrite(dir_pin,LOW); /// WE DON'T KNOW WHY IT IS LOW {dir pin HIGH means B in the motor Driver is getting the signal}
  //Serial.println("hello cw");
}




void motor::anticlockwise()
{
  digitalWrite(dir_pin,HIGH);
  //Serial.println("hello acw");
}




void motor::pwm(int input_pwm)
{
  analogWrite(pwm_pin,input_pwm);
  //Serial.println("hi");
}




motor motorFL(dir_fl,pwm_fl);
motor motorFR(dir_fr,pwm_fr);
motor motorBR(dir_br,pwm_br);
motor motorBL(dir_bl,pwm_bl);




void setup()
{
  Serial.begin(9600);
}








void loop()
{
  Serial.println("Enter pwm: ");
  while(Serial.available()==0);
  input_pwm = Serial.parseInt();
  input_pwm = constrain(input_pwm,0,255);




  Serial.println("Enter choice: ");
  while(Serial.available()==0);
  choice = Serial.parseInt();


  call_choice(choice);




  if((prev_choice == choice)||(prev_choice == 0))               //rover is moving in same direction
  {
    pwm_adjust_for_all_wheels(input_pwm);
  }
  else                                    //rover is changing directions
  {
    call_choice(prev_choice);
    pwm_adjust_for_all_wheels(0);
    prev_pwm = 0;
    call_choice(choice);
    pwm_adjust_for_all_wheels(input_pwm);
  }


  prev_choice = choice;
  prev_pwm = input_pwm;


}














void backward()    
{
  motorFR.anticlockwise();
  motorBR.anticlockwise();




  motorFL.clockwise();
  motorBL.clockwise();
}




void forward()
{
  motorFR.clockwise();
  motorBR.clockwise();




  motorFL.anticlockwise();
  motorBL.anticlockwise();
}




void spotleft()
{
  motorFR.clockwise();
  motorBR.clockwise();




  motorFL.clockwise();
  motorBL.clockwise();
}




void spotright()
{
  motorFR.anticlockwise();
  motorBR.anticlockwise();




  motorFL.anticlockwise();
  motorBL.anticlockwise();
}


void pwm_adjust_for_all_wheels(int passed_pwm)
{
  int adjusted_pwm;
  if(prev_pwm>=passed_pwm)
  {
    for(adjusted_pwm = prev_pwm;adjusted_pwm>=passed_pwm;adjusted_pwm--)
    {
      delay(5);
      motorBR.pwm(adjusted_pwm);
      motorFL.pwm(adjusted_pwm);
      motorBL.pwm(adjusted_pwm);
      motorFR.pwm(adjusted_pwm);
      //Serial.println(adjusted_pwm);


    }
  }
  else if(prev_pwm<passed_pwm)
  {
    for(adjusted_pwm = prev_pwm;adjusted_pwm<=passed_pwm;adjusted_pwm++)
    {
      delay(5);
      motorBR.pwm(adjusted_pwm);
      motorFL.pwm(adjusted_pwm);
      motorBL.pwm(adjusted_pwm);
      motorFR.pwm(adjusted_pwm);
      //Serial.println(adjusted_pwm);
    }
  }


}


void call_choice(int ch)
{
  switch(ch)
  {
    case 1:
      forward();
      break;
    case 2:
      backward();
      break;
    case 3:
      spotleft();
      break;
    case 4:
      spotright();
      break;
    case 0: //change this in ros code
	forward(); //setting some default direction
      input_pwm = 0;
  }
}
