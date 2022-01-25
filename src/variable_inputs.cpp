#include <ros/ros.h>
#include "std_msgs/String.h"
#include "std_msgs/UInt16.h"
#include <string>
#include <iostream>
#include <cmath>
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Pose2D.h"
#include "kinmatic_model/model_state.h"
kinmatic_model::model_state msg;
float cg_velocity=0;
float steering_angle=0;
int main(int argc, char **argv){

ros::init(argc, argv, "variable_inputs");
ros::NodeHandle n;
ros::Publisher node1_pub = n.advertise<kinmatic_model::model_state>("test", 1000);
ros::Rate loop_rate(1);

while(ros::ok()){
std::cout<<"enter parameters"<<std::endl;
std::cin>>cg_velocity;
std::cin>>steering_angle;
msg.cd_velocity=cg_velocity;
msg.steering_wheel=steering_angle;
node1_pub.publish(msg);
ros::spinOnce();
loop_rate.sleep();


}
}
