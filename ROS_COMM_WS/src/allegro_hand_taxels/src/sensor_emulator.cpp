#include <ros/ros.h>
#include <std_msgs/String.h>
#include <serial_example/TactileData.h>

uint32_t timestamp = 1;
uint8_t sensor_num = 1;
std::vector<uint16_t> data {1};
#define SENSORS_NUM 4

int main(int argc, char **argv)
{
  ros::init(argc, argv, "sensor_emulator");
  ros::NodeHandle nh;
  serial_example::TactileData msg;
  std::array<ros::Publisher,SENSORS_NUM> publishers;
  for (size_t i = 0; i < SENSORS_NUM; i++)
  {
   publishers[i] = nh.advertise<serial_example::TactileData>("sensor_"+std::to_string(i+1)+"_readings", 1000);
  }
   
  ros::Rate rate(10);
  while (ros::ok()){
  msg.timestamp = timestamp;
  msg.sensor_num = sensor_num;
  msg.data = data;
  for (size_t i = 0; i < SENSORS_NUM; i++)
  {
   publishers[i].publish(msg);
  }
    ros::spinOnce();
    rate.sleep();
  }


  return 0;
}
// %EndTag(FULLTEXT)%