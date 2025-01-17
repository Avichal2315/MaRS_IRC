#include "ros/ros.h"
#include "std_msgs/Int16.h"
#include "sensor_msgs/Joy.h"



int drive_speed = 0; 
int manipulator_pwm = 0;
int drive_direction = 0; 
int manipulator_choice = 0;


class JoyControl {
public:
   JoyControl() {
       ros::NodeHandle nh;

       


       // subscribe to joystick inputs
       joy_subscriber_ = nh.subscribe("/joy", 10, &JoyControl::joyCallback, this);


       // publishers for drive and manipulator
       drive_speed_publisher_ = nh.advertise<std_msgs::Int16>("/rover/pwm", 10, true);
       drive_direction_publisher_ = nh.advertise<std_msgs::Int16>("/rover/choice", 10, true);
       manipulator_pwm_publisher_ = nh.advertise<std_msgs::Int16>("/manipulator/pwm", 10, true);
       manipulator_choice_publisher_ = nh.advertise<std_msgs::Int16>("/manipulator/choice", 10, true);
        //publisher for steer
       steer_publisher_ = nh.advertise<std_msgs::Int16>("/rover/steer", 10, true);
   }


   void joyCallback(const sensor_msgs::Joy::ConstPtr& msg) {
       std_msgs::Int16 drive_speed_msg;
       std_msgs::Int16 drive_direction_msg;
       std_msgs::Int16 manipulator_pwm_msg;
       std_msgs::Int16 manipulator_choice_msg;
       std_msgs::Int16 steer_msg;


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
           drive_speed = std::max(drive_speed - 5, 0);
           ROS_INFO("Drive: Decrease Speed: %d", drive_speed);
       } else if (msg->buttons[11] == 1) { 
           drive_speed = std::min(drive_speed + 5, 255);
           ROS_INFO("Drive: Increase Speed: %d", drive_speed);
       }else if(msg->buttons[8] == 1){
            steer_msg.data = 1;
            ROS_INFO("Spot turn");
       }else if(msg->buttons[9] == 1){
            steer_msg.data = 2;
            ROS_INFO("Spot turn reverse");
            }


       // manipulator's pwm
       if (msg->buttons[7] == 1) {
           manipulator_pwm = std::min(manipulator_pwm + 5, 255);
           ROS_INFO("Manipulator: Increase PWM: %d", manipulator_pwm);
       } else if (msg->buttons[6] == 1) { 
           manipulator_pwm = std::max(manipulator_pwm - 5, 0);
           ROS_INFO("Manipulator: Decrease PWM: %d", manipulator_pwm);
       }


       if (msg->axes[0] == 0 && msg->axes[1] == 0 && msg->axes[2] == 0 && msg->axes[3] == 0 && msg->axes[6] == 0 && msg->axes[7] == 0) {
           manipulator_choice = 0;
       } else {
           // Manipulator control
         //3,2 are right side knob and 1,0 are left
           if (msg->axes[1] == +1) { 
               manipulator_choice = 1;
               ROS_INFO("Manipulator: LA1 Forward");
           } else if (msg->axes[1] ==-1) { 
               manipulator_choice = 2;
               ROS_INFO("Manipulator: LA1 Backward");
           } else if (msg->axes[3] == +1) {
               manipulator_choice = 3;
               ROS_INFO("Manipulator: LA2 Forward");
           } else if (msg->axes[3] ==-1) {
               manipulator_choice = 4;
               ROS_INFO("Manipulator: LA2 Backward");
           } else if (msg->axes[2] == +1) {
               manipulator_choice = 5;
               ROS_INFO("Manipulator: Belt Clockwise");
           } else if (msg->axes[2] ==-1) {
               manipulator_choice = 6;
               ROS_INFO("Manipulator: Belt Anti-Clockwise");                                    
           } else if (msg->axes[0] == -1) {
               manipulator_choice = 7;
               ROS_INFO("Manipulator: Base Clockwise");
           } else if (msg->axes[0] ==+1) {
               manipulator_choice = 8;
               ROS_INFO("Manipulator: Base Anti-Clockwise"); 
           }
            else if(msg->axes[6]== +1){
            manipulator_choice = 9;
            ROS_INFO("Manipulator: Bevel_gear Clockwise");
           }
           else if(msg->axes[6]== -1){
            manipulator_choice = 10;
            ROS_INFO("Manipulator: Bevel_gear Anti-Clockwise");
           }
           else if(msg->axes[7]== +1){
            manipulator_choice = 11;
            ROS_INFO("Manipulator: Gripper Clockwise");
           }
           else if(msg->axes[7]== -1){      
            manipulator_choice = 12;
            ROS_INFO("Manipulator: Gripper Anti-Clockwise");
           }
       }
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
};


int main(int argc, char **argv) {
   ros::init(argc, argv, "joy_control_node");
   ROS_INFO("Joystick rover and manipulator control node initialized.");


   JoyControl joy_control;
   ros::spin();


   return 0;
}
