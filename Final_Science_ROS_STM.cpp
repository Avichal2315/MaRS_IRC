#include <ros.h>
#include <Arduino.h>
// #include <Servo.h>
#include <std_msgs/Int16.h>   //check if other code have int 16 or 32 if it is INT32 change these lines from 16 to 32
                             //line 4,21,22,30,31,282,287






ros::NodeHandle nh;


// Motor Driver Pulse per Revolution 3200(1/16 step)
// So for rotation of 30 degrees 267(approx) steps
// Or for rotation of 15 degrees 134(approx) steps
int step=134;
int STEP = 67;
int one_step =14;


// const int pulse_pin = PA0;
// const int dirPin = PA1;
// const int enPin = PA2;




void ChoiceCallback(const std_msgs::Int16 &msg);  //callback func. decleration
void PWMCallback(const std_msgs::Int16 &msg);  //callback func. decleration




// Servo Pins
//****PIN NOT FINALISED  */
// Servo myServo;        // Create a Servo object
// const int servoPin = PB3;  // Pin connected to the servo signal


ros::Subscriber<std_msgs::Int16> choice_sub("/manipulator/choice", ChoiceCallback); //subscriber for choice
ros::Subscriber<std_msgs::Int16> pwm_sub("/manipulator/pwm", PWMCallback); //subscriber for pwm






class LA
{
 private:
   int pwm_pin;
   int dir_pin;
  public:
   LA(int, int);
   void assign_pwm(int);
   void forward();
   void backward();
   void stop();
};


LA::LA(int DIR_PIN, int PWM_PIN)
{
 dir_pin = DIR_PIN;
 pwm_pin = PWM_PIN;
 pinMode(dir_pin,OUTPUT);
 pinMode(pwm_pin,OUTPUT);
 digitalWrite(dir_pin,LOW);
 analogWrite(pwm_pin,0);
}


void LA::assign_pwm(int pwm)
{
 analogWrite(pwm_pin,pwm);
}


void LA::forward()
{
 digitalWrite(dir_pin,HIGH);
}


void LA::backward()
{
 digitalWrite(dir_pin,LOW);
}


void LA::stop()
{
 digitalWrite(dir_pin,LOW);
 analogWrite(pwm_pin,0);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// **** Motor Class


class motor
{
 private:
   int pwm_pin;
   int dir_pin;
  public:
   motor(int, int);
   void assign_pwm(int);
   void CW();
   void ACW();
   void stop();
};


motor::motor(int DIR_PIN, int PWM_PIN)
{
 dir_pin = DIR_PIN;
 pwm_pin = PWM_PIN;
 pinMode(dir_pin,OUTPUT);
 pinMode(pwm_pin,OUTPUT);
 digitalWrite(dir_pin,LOW);
 analogWrite(pwm_pin,0);
}


void motor::assign_pwm(int pwm)
{
 analogWrite(pwm_pin,pwm);
}


void motor::CW()
{
 digitalWrite(dir_pin,HIGH);
}


void motor::ACW()
{
 digitalWrite(dir_pin,LOW);
}


void motor::stop()
{
 digitalWrite(dir_pin,LOW);
 analogWrite(pwm_pin,0);
}








/////////////////////////////////////////////////////////////////////////////
// **** Stepper Class


class stepper // will rotate the stepper 15degree in one go
{
 private:
   int pulse_pin;
   int dir_pin;
   int enable_pin;
  public:
   stepper(int, int,int);
   // void assign_pwm(int);/
   void CW();
   void ACW();
   void stop();
   void CW1();
   void ACW1();
};


stepper::stepper(int DIR_PIN, int PULSE_PIN, int ENABLE_PIN)
{
 dir_pin = DIR_PIN;
 pulse_pin = PULSE_PIN;
 enable_pin = ENABLE_PIN;


 pinMode(dir_pin,OUTPUT);
 pinMode(pulse_pin,OUTPUT);
 pinMode(enable_pin,OUTPUT);


 digitalWrite(enable_pin,LOW);
 digitalWrite(dir_pin,LOW);


 digitalWrite(pulse_pin,LOW);
}






void stepper::CW() // this if for 15 degree turn
{
digitalWrite(dir_pin,HIGH); // Enables the motor to move in a particular direction


 for(int x = 0; x < step; x++) {  //counts and rotates for specific degrees, according to the number of steps
   digitalWrite(pulse_pin,HIGH);
   delayMicroseconds(1000);
   digitalWrite(pulse_pin,LOW);
   delayMicroseconds(1000);
 }
 }


void stepper::CW1() // this if for 5 degree turn
{
digitalWrite(dir_pin,HIGH); // Enables the motor to move in a particular direction


 for(int x = 0; x < STEP; x++) {  //counts and rotates for specific degrees, according to the number of steps
   digitalWrite(pulse_pin,HIGH);
   delayMicroseconds(1000);
   digitalWrite(pulse_pin,LOW);
   delayMicroseconds(1000);
 }
 }


void stepper::ACW() // 15 degree rotation
{
 digitalWrite(dir_pin,LOW); // Enables the motor to move in a particular direction


 for(int x = 0; x < step; x++) {  //counts and rotates for specific degrees, according to the number of steps
   digitalWrite(pulse_pin,HIGH);
   delayMicroseconds(1000);
   digitalWrite(pulse_pin,LOW);
   delayMicroseconds(1000);
 }
 // stop();
 }


void stepper::ACW1() // 5 degree rotation
{
 digitalWrite(dir_pin,LOW); // Enables the motor to move in a particular direction


 for(int x = 0; x < STEP; x++) {  //counts and rotates for specific degrees, according to the number of steps
   digitalWrite(pulse_pin,HIGH);
   delayMicroseconds(1000);
   digitalWrite(pulse_pin,LOW);
   delayMicroseconds(1000);
 }
 // stop();
 }


void stepper::stop()
{
 digitalWrite(enable_pin,LOW);
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




// // Servo Motor class




// class servo // will rotate the stepper 15degree in one go
// {
//   private:
//     int servo_pin;
 //   public:
//     servo(int);
//     // void assign_pwm(int);/
//     void rotate();   
//     void back();
// };


// servo::servo(int SERVO_PIN)
// {
//   servo_pin  = SERVO_PIN;


//   pinMode(servo_pin,OUTPUT);
//   myServo.attach(servoPin);         // Attach the servo to the specified pin
//   myServo.write(0); 


// }


// void servo::rotate()
// {
//     myServo.write(180); 


// }


// void servo:: back(){
//   myServo.write(0);
// }












// Pins are not yet finalised
LA linear_actuator(PB5, PB4); // M2(MRW1)
motor motor1(PB15,PB14); //dir: PB15 , pwm: PB14 ***M4(BRW1)
motor motor2(PC15,PA0);  // dir: PC15, pwm: PA0  *** M5(FLW1)


stepper stepper1(PA1,PA2,PB9);  // M3(MLW1)-{dir pin: PA1, pulse pin: PA2}, M6(FRW1): dir_pin->enable pin


// servo servo1(PB8); // servo_pin: M6(FLW1)-pwm_pin






int choice;
int pwm; //common pwm for all links. if needed, make one pwm for actuators and one for motors later


void PWMCallback(const std_msgs::Int16 &msg) {
pwm = msg.data;
}




void ChoiceCallback(const std_msgs::Int16 &msg) {
choice = msg.data;


switch(choice)
 {
  
   // Linear Actuator
   case 1: // up
   
   
     linear_actuator.assign_pwm(pwm); linear_actuator.forward(); break;


   case 2:  //down
    
   
     linear_actuator.assign_pwm(pwm); linear_actuator.backward(); break;




   // Motor to Linear Actuator (TOP)
   case 3:  // up
    
     motor1.assign_pwm(pwm); motor1.CW(); break;


   case 4:  // Down
    
   
     motor1.assign_pwm(pwm); motor1.ACW(); break;
  




   // Motor to Move Drill
   case 5:  // to earth
   
    
     motor2.assign_pwm(pwm); motor2.CW(); break;


   case 6:  // away from earth
    
    
     motor2.assign_pwm(pwm); motor2.ACW(); break;


  
   case 7: 
     stepper1.CW(); break;


   case 8: 
     stepper1.ACW(); break;
  
   case 9: 
     stepper1.CW1(); break;


   case 10: 
     stepper1.ACW1(); break;


   case 0:
     linear_actuator.stop(); motor1.stop(); motor2.stop();  break; // no stepper stop needed


   default:
     linear_actuator.stop(); motor1.stop(); motor2.stop();  break; // no stepper stop needed
 }
}










void setup()
{
 Serial.begin(9600);
  nh.initNode();
 nh.subscribe(pwm_sub);
 nh.subscribe(choice_sub);
 // Serial.println("pwm:");
 // while (Serial.available() == 0);
 // pwm = Serial.parseInt();
 // pwm = 80;
}


void loop()
{


 nh.spinOnce();
 delay(10);
 // link1.assign_pwm(pwm);
 // link2.assign_pwm(pwm);
 //link1.forward();


 // Serial.println("choice:");
 // while (Serial.available() == 0);
 // choice = Serial.parseInt();




 }
