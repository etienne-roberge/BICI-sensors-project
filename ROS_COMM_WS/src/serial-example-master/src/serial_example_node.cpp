/* This code receives an array of bytes from the USB serial port and stores it in a float message structure that is published to a ros topic*/

#include <ros/ros.h>
#include <unistd.h> 
#include <stdio.h> 
#include <string.h>
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h>
#include <iostream>
#include <std_msgs/Float32MultiArray.h>
#include <vector>

int n;// The number of bytes read
const int Buffer_size = 50; //Buffer size is set to a constant for testing
unsigned char read_buf [Buffer_size];
std_msgs::Float32MultiArray msg;

int main (int argc, char** argv){
    ros::init(argc, argv, "serial_example_node");
    ros::NodeHandle nh;

    ros::Publisher read_pub = nh.advertise<std_msgs::Float32MultiArray>("read", 1000);
    int serial_port = open("/dev/ttyUSB0", O_RDWR); //The serial port has to be properly renamed in certain cases
    
    ros::Rate loop_rate(5);
    while(ros::ok()){

        ros::spinOnce();

	n = read(serial_port, &read_buf, sizeof(read_buf));
	/* The behaviour of read() (e.g. does it block?,
	how long does it block for?) depends on the configuration settings, specifically VMIN and VTIME, which can be used in the future if necessary */
	/*printf("Slave Address: %02X \n",read_buf[0]);*/
    for(int i = 0;i<Buffer_size; i++)
    {
        msg.data.push_back(read_buf[i]);
    }
	
    
    read_pub.publish(msg);
        }
        loop_rate.sleep();

    }


