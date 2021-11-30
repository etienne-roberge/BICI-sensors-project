#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <iostream>
#include <fstream>
using namespace std;
std::ofstream myfile;
bool file_start = true;
sensor_msgs::JointState js_merged;

void callback(const sensor_msgs::JointState::ConstPtr &msg_in) {

  js_merged = *msg_in;
    if (file_start)
  {
    myfile << "" << ',';
    for (size_t i = 0; i < js_merged.name.size(); i++)
  {
    myfile << js_merged.name[i] << ',';
  }
  myfile << '\n';
  file_start = false;
  }
  myfile << "position" << ',';
  for (size_t i = 0; i < js_merged.name.size(); i++)
  {
    myfile << js_merged.position[i] << ',';
  }
  myfile << '\n';
  myfile << "velocity" << ',';
  for (size_t i = 0; i < js_merged.name.size(); i++)
  {
    myfile << js_merged.velocity[i] << ',';
  }
  myfile << '\n';
  myfile << "effort" << ',';
    for (size_t i = 0; i < js_merged.name.size(); i++)
  {
    
    myfile << js_merged.effort[i] << ',';
  }
  myfile << '\n';

}

int main(int argc, char **argv){

  ros::init(argc, argv, "joint_states_saver");

  ros::NodeHandle nh;
  myfile.open ("joints_data.csv");
  ros::Subscriber sub_ur5 = nh.subscribe<sensor_msgs::JointState>( "ur5_allegro/joint_states", 1, &callback);

  ros::spin();
  if(!ros::ok()){
    myfile.close();
  }

  return 0;

}