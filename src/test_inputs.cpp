#include <ros/ros.h>
#include "std_msgs/String.h"
#include "std_msgs/UInt16.h"
#include <string>
#include <iostream>
#include <cmath>
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Pose2D.h"
#include "kinmatic_model/model_state.h"
kinmatic_model::model_state msg2;
float velocity=0;
float angle=0;
void subCallback(const kinmatic_model::model_state::ConstPtr& msg){
velocity=msg->cd_velocity;
angle=msg->steering_wheel;
msg2.cd_velocity=velocity;
msg2.steering_wheel=angle;

}
int main(int argc, char **argv){
msg2.cd_velocity=0;
msg2.steering_wheel=0;
ros::init(argc, argv, "test_inputs");
ros::NodeHandle n;
ros::Subscriber sub = n.subscribe("test", 1, subCallback);
ros::Publisher node2_pub = n.advertise<kinmatic_model::model_state>("/model_input", 1000);
ros::Rate loop_rate(1);
while(ros::ok()){
node2_pub.publish(msg2);
ros::spinOnce();
loop_rate.sleep();

}
}
