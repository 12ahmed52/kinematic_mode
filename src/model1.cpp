#include <ros/ros.h>
#include "std_msgs/String.h"
#include "std_msgs/UInt16.h"
#include <string>
#include <iostream>
#include <cmath>
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Pose2D.h"
#include "kinmatic_model/model_state.h"

using namespace std;
#define pi 3.14159265359
  
geometry_msgs::Pose2D model_msg_output;

class bicycle
{
private:
public:
    
    float beta,theta_dot,theta,theta_dot_prev,lr,l,sample_time,x_cg_dot,x_cg_dot_prev,y_cg_dot,y_cg_dot_prev, input[3],output[3];
    float count;
    bicycle();
    ~bicycle();
    void forward_kinematics();
    void print_output();
};

bicycle::bicycle()
{
 beta=0;     //slipping angel
 theta_dot=0; 
 theta=0;    
 theta_dot_prev=0;
 lr=0.5;
 l=1;
 sample_time=0.01;
 //input[]={0,0,0};
 //output[]={0,0,0};
count=0;
}

bicycle::~bicycle()
{
}
void bicycle::forward_kinematics()
{
//beta=atan(   (  lr*(tan(input[2])/l)  )  );
//theta_dot=input[0]*cos(beta)*tan(input[2])/l;
//theta=(theta_dot+theta_dot_prev)*sample_time;
//theta_dot_prev=theta_dot;
//x_cg_dot=input[0]*cos(theta+beta);
//output[0]=(x_cg_dot+x_cg_dot_prev)*sample_time;
// x_cg_dot_prev=x_cg_dot;
//y_cg_dot=input[0]*sin(theta+beta);
//output[1]=(y_cg_dot+y_cg_dot_prev)*sample_time;
//y_cg_dot_prev=y_cg_dot;
//output[2]=theta;

beta=atan(   (  lr*(tan(input[2])/l)  )  );
x_cg_dot=input[0]*cos(theta+beta);
y_cg_dot=input[0]*sin(theta+beta);
theta_dot=input[0]*cos(beta)*tan(input[2])/l;      
output[0]+=x_cg_dot*sample_time;      
output[1]+=y_cg_dot*sample_time;      
theta+=theta_dot*sample_time;
output[2]=theta;
output[2]=remainder(theta,2*pi);

}

void bicycle::print_output()
{   
    cout<<"------"<<count++<<"------"<<endl;
    cout<<"x= "<<output[0]<<endl;
    cout<<"y= "<<output[1]<<endl;
    cout<<"theta= "<<output[2]<<endl;

   

}



float cg_velosity=0.0;
float steering_angel=0;
bicycle bike1;


class sub_pub_node
{
public:

  sub_pub_node()
  {
	model_input_sub = handel.subscribe("/model_input", 10,&sub_pub_node::model_input_callback,this );

	model_out_pub = handel.advertise<geometry_msgs::Pose2D>("/model_output", 10);

  
  }


  void model_input_callback(const kinmatic_model::model_state::ConstPtr& msg)
  {
    //stringstream ss(msg->data);
     //ss>>name>>age>>height;
    //ROS_INFO("naem: [%s]",name.c_str());
    
    bike1.input[0]= msg->cd_velocity;
    bike1.input[2]=msg->steering_wheel;
    ROS_INFO("velocity: [%f]",msg->cd_velocity);
    ROS_INFO("steering angel: [%f]",msg->steering_wheel);
    cout<<"============================"<<endl;

    bike1.forward_kinematics();
   // bike1.print_output();

    model_msg_output.x= bike1.output[0];
    model_msg_output.y= bike1.output[1];
    model_msg_output.theta= bike1.output[2];
    

    model_out_pub.publish(model_msg_output);
  }

private:
  ros::NodeHandle handel; 
  ros::Publisher model_out_pub;
  
  ros::Subscriber model_input_sub;

  

	
	

};




int main(int argc, char **argv)
{
    
  ros::init(argc, argv, "data_processing");

  sub_pub_node sub_pub_object;

  ros::spin();
  
  

  return 0;
}