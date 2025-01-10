//ROS integrated Fake PID code



#include <ros.h>
#include <Arduino.h>
#include <std_msgs/Int16.h>

//Follow the below mentioned convention
//Make sure if you press A, the wheel goes clockwise with respect to the front of the motor
//When high is to the direction pin, B gets the signal.
//The above point was found out experimentally


//Function declaration

// Motor Driver Pin Declaration
#define dir_fl PC15    
#define pwm_fl PA0

#define dir_fr PB9
#define pwm_fr PB8

#define dir_bl PA5
#define pwm_bl PA6

#define dir_br PB15
#define pwm_br PB14


int choice = 0;
int prev_choice = 0;
int input_pwm = 0;
int prev_pwm = 0;




// ROS Node Handle
ros::NodeHandle nh;



// Callback Function Declarations
void driveCallback(const std_msgs::Int16 &msg);
void ChoiceCallback(const std_msgs::Int16 &msg);





// ROS Subscribers
ros::Subscriber<std_msgs::Int16> pwm_sub("/rover/pwm", driveCallback);
ros::Subscriber<std_msgs::Int16> direction_sub("/rover/choice", ChoiceCallback);







// Motor Class
class motor {
  private:
    int dir_pin;
    int pwm_pin;

  public:
    motor(int pin1, int pin2);
    void clockwise();
    void anticlockwise();
    void pwm(int input_pwm);
};

// Motor Class Implementation
motor::motor(int pin1, int pin2) {
  dir_pin = pin1;
  pwm_pin = pin2;    
  pinMode(dir_pin, OUTPUT);
  pinMode(pwm_pin, OUTPUT);
}




void motor::clockwise() {// Viewing the rover form the right side with respect to front of the rover
  digitalWrite(dir_pin, LOW);/// WE DON'T KNOW WHY IT IS LOW {dir pin HIGH means B in the motor Driver is getting the signal}
  //Serial.println("hello cw");
}




void motor::anticlockwise() {
  digitalWrite(dir_pin, HIGH);
}

void motor::pwm(int input_pwm) {
  analogWrite(pwm_pin, input_pwm);
}

// Motor Objects
motor motorFL(dir_fl, pwm_fl);
motor motorFR(dir_fr, pwm_fr);
motor motorBR(dir_br, pwm_br);
motor motorBL(dir_bl, pwm_bl);





void forward();
void backward();
void spotleft();
void spotright();
void pwm_adjust_for_all_wheels(int);
void call_choice(int);

// ROS Callbacks
void driveCallback(const std_msgs::Int16 &msg) {
 
 input_pwm = msg.data;
  input_pwm = constrain(input_pwm,0,255);
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

void ChoiceCallback(const std_msgs::Int16 &msg) {
choice = msg.data;
//   if (prev_choice != new_choice) {
//     pwm_adjust_for_all_wheels(0); // Stop motors before changing direction
//     prev_pwm = 0;
//     call_choice(new_choice);
//   }
//   prev_choice = new_choice;
 //call_choice(choice);

// if((prev_choice == choice)||(prev_choice == 0))               //rover is moving in same direction
//   {
//     pwm_adjust_for_all_wheels(input_pwm);
//   }
//   else                                    //rover is changing directions
//   {
//     call_choice(prev_choice);
//     pwm_adjust_for_all_wheels(0);
//     prev_pwm = 0;
//     call_choice(choice);
//     pwm_adjust_for_all_wheels(input_pwm);
//   }

//   prev_choice = choice;
//   prev_pwm = input_pwm;


}




// Movement Functions
void forward() {
  motorFR.clockwise();
  motorBR.clockwise();
  motorFL.anticlockwise();
  motorBL.anticlockwise();
}

void backward() {
  motorFR.anticlockwise();
  motorBR.anticlockwise();
  motorFL.clockwise();
  motorBL.clockwise();
}

void spotleft() {
  motorFR.clockwise();
  motorBR.clockwise();
  motorFL.clockwise();
  motorBL.clockwise();
}

void spotright() {
  motorFR.anticlockwise();
  motorBR.anticlockwise();
  motorFL.anticlockwise();
  motorBL.anticlockwise();
}

void pwm_adjust_for_all_wheels(int passed_pwm) {
  int adjusted_pwm;
  if (prev_pwm >= passed_pwm) {
    for (adjusted_pwm = prev_pwm; adjusted_pwm >= passed_pwm; adjusted_pwm--) {
      delay(5);
      motorBR.pwm(adjusted_pwm);
      motorFL.pwm(adjusted_pwm);
      motorBL.pwm(adjusted_pwm);
      motorFR.pwm(adjusted_pwm);
    }
  } else {
    for (adjusted_pwm = prev_pwm; adjusted_pwm <= passed_pwm; adjusted_pwm++) {
      delay(5);
      motorBR.pwm(adjusted_pwm);
      motorFL.pwm(adjusted_pwm);
      motorBL.pwm(adjusted_pwm);
      motorFR.pwm(adjusted_pwm);
    }
  }
  prev_pwm = passed_pwm;
}



void call_choice(int ch) {
  switch (ch) {
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
    case 0: // Stop
      pwm_adjust_for_all_wheels(0);
      break;
    default:
      Serial.println("Invalid choice");
      break;
  }
}
















// Setup Function
void setup() {
  nh.initNode();
  nh.subscribe(pwm_sub);
  nh.subscribe(direction_sub);

 // Serial.begin(9600);
  pinMode(PB7, OUTPUT);
  digitalWrite(PB7, LOW);

 // Serial.println("Rover ROS Node Initialized");
}

// Main Loop
void loop() {
  nh.spinOnce();
  delay(10);


//removed all unnecessary code from loop and initially input was fed using serial monitor( was causing problem) but now it is fed using ROS.





}
