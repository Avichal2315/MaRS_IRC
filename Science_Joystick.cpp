#include "ros/ros.h"
#include "std_msgs/Int16.h"
#include "sensor_msgs/Joy.h"



int drive_speed = 0; 
int manipulator_pwm = 0;
int drive_direction = 0; 
int manipulator_choice = 0;
int individual_control = 0;






class JoyControl {
public:
   JoyControl() {
       ros::NodeHandle nh;
       
       

       


       // subscribe to joystick inputs
       joy_subscriber_ = nh.subscribe("/joy", 10, &JoyControl::joyCallback, this);


       // publishers for drive and manipulator
       drive_speed_publisher_ = nh.advertise<std_msgs::Int16>("/move", 10, true);
       drive_direction_publisher_ = nh.advertise<std_msgs::Int16>("/rover/choice", 10, true);
       manipulator_pwm_publisher_ = nh.advertise<std_msgs::Int16>("/manipulator/pwm", 10, true);
       manipulator_choice_publisher_ = nh.advertise<std_msgs::Int16>("/manipulator/choice", 10, true);
        //publisher for steer
       steer_publisher_ = nh.advertise<std_msgs::Int16>("/rover/steer", 10, true);
   //    individual_control_publisher_ = nh.advertise<std_msgs::Int16>("/rover/individual_control", 10, true);
   }


   void joyCallback(const sensor_msgs::Joy::ConstPtr& msg) {
       std_msgs::Int16 drive_speed_msg;
       std_msgs::Int16 drive_direction_msg;
       std_msgs::Int16 manipulator_pwm_msg;
       std_msgs::Int16 manipulator_choice_msg;
       std_msgs::Int16 steer_msg;
      // std_msgs::Int16 individual_control_msg;

       // Drive control
     //start button-increase drive's pwm and select button is decrease
       if(msg->buttons[12] == 1){
        drive_direction = 0;
        ROS_INFO("Rover: Stop");}
       else if (msg->buttons[0] == 1) { 
           drive_direction = 2; 
           ROS_INFO("Drive: Backward");
       } else if (msg->buttons[4] == 1) { 
           drive_direction = 1;
           ROS_INFO("Drive: Forward");
       } else if (msg->buttons[1] == 1) {
           drive_direction = 4;
           ROS_INFO("Drive: Right");
       } else if (msg->buttons[3] == 1) {
           drive_direction = 3;
           ROS_INFO("Drive: Left");
       } else if (msg->buttons[10] == 1) { 
           drive_speed = drive_speed - 5;
           ROS_INFO("Drive: Decrease Speed: %d", drive_speed);
       } else if (msg->buttons[11] == 1) { 
           drive_speed = drive_speed + 5;
           ROS_INFO("Drive: Increase Speed: %d", drive_speed);
       }else if ((msg->buttons[6] == 1) && (msg->buttons[7] == 1)){ //steer code starts
       
              steer_msg.data = 5;
            ROS_INFO("both wheels left"); //left
            }else if ((msg->buttons[8] == 1) && (msg->buttons[9] == 1)){
            
              steer_msg.data = 6;
            ROS_INFO("both wheels right"); //right
            
       }else if(msg->buttons[9] == 1){
       
            steer_msg.data = 2;
            ROS_INFO("right wheel right"); //right
            
            }else if(msg->buttons[7] == 1){
            
            steer_msg.data = 1;
            ROS_INFO("right wheel left"); //left
            
            }else if(msg->buttons[6] == 1){
            
              steer_msg.data = 3;
            ROS_INFO("left wheel left"); //left
            
            }else if(msg->buttons[8] == 1){
            
              steer_msg.data = 4;
            ROS_INFO("left wheel right"); //right
            
            }


       // manipulator's pwm
       //if (msg->buttons[7] == 1) {
           //manipulator_pwm = std::min(manipulator_pwm + 5, 255);
           //ROS_INFO("Manipulator: Increase PWM: %d", manipulator_pwm);
       //} else if (msg->buttons[6] == 1) { 
           //manipulator_pwm = std::max(manipulator_pwm - 5, 0);
           //ROS_INFO("Manipulator: Decrease PWM: %d", manipulator_pwm);
       if (msg->axes[0] == 0 && msg->axes[1] == 0 && msg->axes[2] == 0 && msg->axes[3] == 0 && msg->axes[6] == 0 && msg->axes[7] == 0) {
           manipulator_choice = 0;
       } else {
           // Manipulator control
         //3,2 are right side knob and 1,0 are left
           if (msg->axes[3] == +1) {
               manipulator_choice = 1;
               ROS_INFO("LA UP");
               manipulator_pwm=200;
           } else if (msg->axes[3] ==-1) {
               manipulator_choice = 2;
               ROS_INFO("LA Down");
               manipulator_pwm=200;
           } else if (msg->axes[1] == +1) {
               manipulator_choice = 3;
               ROS_INFO("Motor1 Up");
               manipulator_pwm=230;
           } else if (msg->axes[1] ==-1) {
               manipulator_choice = 4;
               ROS_INFO("Motor1 Down");
               manipulator_pwm=230;
           } else if (msg->axes[2] == +1) {
               manipulator_choice = 5;
               ROS_INFO("Drill Motor To Earth");
               manipulator_pwm=130;
           } else if (msg->axes[2] ==-1) {
               manipulator_choice = 6;
               ROS_INFO("Drill Motor Away From Earth");
               manipulator_pwm=130;                                    
           } else if (msg->axes[0] == -1) {
               manipulator_choice = 7;
               ROS_INFO("Stepper side 1: 15 degree");
           } else if (msg->axes[0] ==+1) {
               manipulator_choice = 8;
               ROS_INFO("Stepper Side 2 15 degree");
           }
            else if(msg->axes[6]== +1){
            manipulator_choice = 9;
            ROS_INFO("Stepper Side 1: 5 Degree");
           }
           else if(msg->axes[6]== -1){
            manipulator_choice = 10;
            ROS_INFO("Stepper Side 2: 5 Degree");
           }
           else if(msg->axes[7]== +1){
            manipulator_choice = 11;
            ROS_INFO("Nothing");
           }
           else if(msg->axes[7]== -1){      
            manipulator_choice = 12;
            ROS_INFO("Nothing");
           }
       }


      /* if (msg->axes[0] == 0 && msg->axes[1] == 0 && msg->axes[2] == 0 && msg->axes[3] == 0 && msg->axes[6] == 0 && msg->axes[7] == 0) {
           manipulator_choice = 0;
       } else {
           // Manipulator control
         //3,2 are right side knob and 1,0 are left
           if (msg->axes[1] == +1) { 
               manipulator_choice = 1;
               ROS_INFO("Manipulator: First Arm Up");
               manipulator_pwm = 200;
           } else if (msg->axes[1] ==-1) { 
               manipulator_choice = 2;
               ROS_INFO("Manipulator: First Arm Down");
               manipulator_pwm = 200;
           } else if (msg->axes[3] == +1) {
               manipulator_choice = 3;
               ROS_INFO("Manipulator: Second Arm Up");
               manipulator_pwm = 200;
           } else if (msg->axes[3] ==-1) {
               manipulator_choice = 4;
               ROS_INFO("Manipulator: Second Arm Down");
               manipulator_pwm = 200;
           } else if (msg->axes[2] == +1) {
               manipulator_choice = 5;
               ROS_INFO("Manipulator: Belt Up");
               manipulator_pwm = 160;
           } else if (msg->axes[2] ==-1) {
               manipulator_choice = 6;
               ROS_INFO("Manipulator: Belt Down"); 
               manipulator_pwm = 160;                                   
           } else if (msg->axes[0] == -1) {
               manipulator_choice = 8;
               ROS_INFO("Manipulator: Base Right");
               manipulator_pwm= 130;
           } else if (msg->axes[0] ==+1) {
               manipulator_choice = 7;
               ROS_INFO("Manipulator: Base Left"); 
               manipulator_pwm =130;
           }
            else if(msg->axes[6]== +1){
            manipulator_choice = 9;
            ROS_INFO("Manipulator: Bevel_gear Left");
            manipulator_pwm = 160;
           }
           else if(msg->axes[6]== -1){
            manipulator_choice = 10;
            ROS_INFO("Manipulator: Bevel_gear Right");
            manipulator_pwm = 160;
           }
           else if(msg->axes[7]== +1){
            manipulator_choice = 11;
            ROS_INFO("Manipulator: Gripper Close");
            manipulator_pwm = 150;
           }
           else if(msg->axes[7]== -1){      
            manipulator_choice = 12;
            ROS_INFO("Manipulator: Gripper Open");
            manipulator_pwm = 150;
           }
       }*/


          /*while(ros::ok()){
            std::cout<<"individual control "<<std::endl;

            std::cin>> individual_control;
            
          }

          if(individual_control == 1){
            ROS_INFO("FL:  turn");
            individual_control_publisher_.publish(individual_control_msg);
          }else if(individual_control == 2){
            ROS_INFO("FL:  turn reverse");
            individual_control_publisher_.publish(individual_control_msg);
          }else if(individual_control == 3){
            ROS_INFO("FR:  turn");
            individual_control_publisher_.publish(individual_control_msg);
          }else if(individual_control == 4){
            ROS_INFO("FR:  turn reverse");
            individual_control_publisher_.publish(individual_control_msg);
          }else if(individual_control == 5){
            ROS_INFO("RL:  turn");
            individual_control_publisher_.publish(individual_control_msg);
          }else if(individual_control == 6){
            ROS_INFO("RL:  turn reverse");
            individual_control_publisher_.publish(individual_control_msg);
          }else if(individual_control == 7){
            ROS_INFO("RR:  turn");
            individual_control_publisher_.publish(individual_control_msg);
          }else if(individual_control == 8){
            ROS_INFO("RR:  turn reverse");
            individual_control_publisher_.publish(individual_control_msg);
          }
          individual_control_msg.data = individual_control; */

       


       // publish drive msgs
       drive_speed_msg.data = drive_speed;
       drive_direction_msg.data = drive_direction;
       drive_speed_publisher_.publish(drive_speed_msg);
       drive_direction_publisher_.publish(drive_direction_msg);
       // publish manipulator msgs
       manipulator_pwm_msg.data = manipulator_pwm;
       manipulator_choice_msg.data = manipulator_choice;
       manipulator_pwm_publisher_.publish(manipulator_pwm_msg);
       manipulator_choice_publisher_.publish(manipulator_choice_msg);
       steer_publisher_.publish(steer_msg);
        


   }


private:
   ros::Subscriber joy_subscriber_;
   ros::Publisher drive_speed_publisher_;
   ros::Publisher drive_direction_publisher_;
   ros::Publisher manipulator_pwm_publisher_;
   ros::Publisher manipulator_choice_publisher_;
   ros::Publisher steer_publisher_;
   //ros::Publisher individual_control_publisher_;
};


int main(int argc, char **argv) {
   ros::init(argc, argv, "joy_control_node");
   ROS_INFO("Joystick rover and manipulator control node initialized.");

    
    
   JoyControl joy_control;
   ros::spin();


   return 0;
}
