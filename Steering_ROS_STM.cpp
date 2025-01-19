//Steering code (with Spotturn and reverse Spotturn)


#include<Arduino.h>
#include<ros.h>
#include<std_msgs/Int16.h>

// function declaration pins need to check for the steering motor now it is same as drive 

ros::NodeHandle nh;

std_msgs::Int16 steer_msg;

  int command; 
  int flag = 0;











void spotturn();
void spotturnrev();
void frontsteerR();
void frontsteerL();
void done();
// defining pins

//front left
#define dir_fl PB9   
#define pwm_fl PB8

//front right
#define dir_fr PA1
#define pwm_fr PA2

//back left
#define dir_bl PA5
#define pwm_bl PA6

//back right
#define dir_br PC15
#define pwm_br PA0

int choice;
int input_pwm = 120;
int present_angle = 0;
int current_angle = 0;
int target_angle = 0;

class motor
{
private:
  int dir_pin;
  int pwm_pin;

public:
  motor(int pin1, int pin2);
  void clockwise();
  void anticlockwise();
  void pwm(int input_pwm);
};

motor::motor(int pin1, int pin2)
{
  dir_pin = pin1;
  pwm_pin = pin2;    
  pinMode(dir_pin, OUTPUT);
  pinMode(pwm_pin, OUTPUT);
}

// all clockwise and anticlockwise dir are assumed to be from the top 
void motor::clockwise() 
{
  digitalWrite(dir_pin, LOW); // need to check its clockwise or anticlockwise
}

void motor::anticlockwise()
{
  digitalWrite(dir_pin, HIGH); // need to check its clockwise or anticlockwise
}

void motor::pwm(int input_pwm)
{
  analogWrite(pwm_pin, input_pwm);
}

motor motorFLS(dir_fl, pwm_fl);
motor motorFRS(dir_fr, pwm_fr);
//motor motorBRS(dir_br, pwm_br);
//motor motorBLS(dir_bl, pwm_bl);



void commandCallback(const std_msgs::Int16& msg) {
     // Process command

  
        choice=msg.data;

//   Serial.println("Enter pwm: ");
//   while(Serial.available() == 0);
//   input_pwm = Serial.parseInt();
//   input_pwm = constrain(input_pwm, 0, 255);

//   Serial.println("Enter choice: ");
//   while(Serial.available() == 0);
  
  

  //Serial.print("Time: ");
  //my_time = millis();
  //Serial.println(my_time);

  // switch (choice)
  // {
  //   case 1:
      

  //       target_angle = 2000;

  //       // int initial_time = millis(), current_time = millis();
  //       // while (current_time - initial_time < 5000)
  //       // {
  //       //   current_time = millis();
  //       // }
  //        // Update safe after action
  //         // Set flag to indicate action taken
  //         break;
      

  //   // case 2:
  //   //   Serial.print(safe);
  //   //   if (safe == 1)  // Ensure the action can only happen after case 1
  //   //   {
        
  //   //     safe = 2;  // Set to 2 after second action
  //   //     flag = 1;  // Set flag to indicate action taken
  //   //     delay(5000);
  //   //     done();
  //   //   }
  //   //   break;

  //    case 2:
  //       target_angle = 0;
  //       break;
  //   //   frontsteerR();
  //   //   motorFRS.pwm(input_pwm);
  //   //   motorFLS.pwm(input_pwm);
  //   //   delay(1000);
  //   //   done();
  //   //   break;

  //   // case 3:
  //   //   frontsteerL();
  //   //   motorFRS.pwm(input_pwm);
  //   //   motorFLS.pwm(input_pwm);
  //   //     delay(1000);
  //   //     done();
  //   //   break;
  // }
  switch(choice)
  {
    case 1: //Right Wheel Right
      motorFRS.clockwise();
      motorFRS.pwm(input_pwm); //Input PWM from joystick
      break;
    case 2: //RIght WHeel Left
      motorFRS.anticlockwise();
      motorFRS.pwm(input_pwm); //Input PWM from joystick
      break; 
    case 3: //Left Wheel Right
      motorFLS.clockwise();
      motorFLS.pwm(input_pwm);
      break;
    case 4: //Left wheel Left
      motorFLS.anticlockwise();
      motorFLS.pwm(input_pwm);
      break;
    case 5://Both Right
      motorFLS.clockwise();
      motorFRS.clockwise();
      motorFLS.pwm(input_pwm);
      motorFRS.pwm(input_pwm);
      break;
    case 6: //Both Left
      motorFLS.anticlockwise();
      motorFRS.anticlockwise();
      motorFLS.pwm(input_pwm);
      motorFRS.pwm(input_pwm);
      break;
    case 0: //Nothing moves
      motorFLS.pwm(0);
      motorFRS.pwm(0);
      break;
  }
}

ros::Subscriber <std_msgs::Int16>  steer_sub("/rover/steer", commandCallback); //rostopic name




// void done()
// {
//   motorFRS.pwm(0);
//   //motorBRS.pwm(0);
//   motorFLS.pwm(0);
//   //motorBLS.pwm(0);
// }

// void frontsteerR()
// {
//   motorFRS.clockwise();
//   motorFLS.clockwise();
// }

// void spotturn()
// {
//   motorFRS.clockwise();
//   //motorBRS.anticlockwise();
//   motorFLS.clockwise();
//   //motorBLS.anticlockwise();
// }

// void frontsteerL()
// {
//   motorFRS.anticlockwise();
//   motorFLS.anticlockwise();
// }

// void spotturnrev()
// {
//   motorFRS.anticlockwise();
//   //motorBRS.clockwise();
//   motorFLS.anticlockwise();
//   ///motorBLS.clockwise();
// }



void setup()
{
 // Serial.begin(9600);
  nh.initNode();
  nh.subscribe(steer_sub);
}


void loop()
{
  // present_angle = millis();
  // if (target_angle - 5 > current_angle)
  // {
  //   spotturn();
  //   motorFRS.pwm(input_pwm*2);
  //   //motorBRS.pwm(input_pwm);
  //   motorFLS.pwm(input_pwm);
  //   //motorBLS.pwm(input_pwm*2);
  //   delay(10);
  //   current_angle += millis() - present_angle;
  // } else if (target_angle + 5 < current_angle)
  // {
  //   spotturnrev();
  //   motorFRS.pwm(input_pwm*2);
  //   //motorBRS.pwm(input_pwm);
  //   motorFLS.pwm(input_pwm);
  //   //motorBLS.pwm(input_pwm*2);
  //   delay(10);
  //   current_angle -= millis() - present_angle;
  // } else
  // {
  //   delay(10);
  //   done();
  // }

  delay(10);
  nh.spinOnce();
}
