/* This code receives an array of bytes from the USB serial port and stores it in a uint16 array that is published to a ros topic*/

#include <ros/ros.h>
#include <unistd.h> 
#include <stdio.h> 
#include <string.h>
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h>
#include <iostream>
#include <std_msgs/UInt16MultiArray.h>
#include <serial_example/TactileData.h>
#include <termios.h>
#include <poll.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <boost/circular_buffer.hpp>

#define PRINT

int n;// The number of bytes read
const int buffer_size = 58; // largest possible message size
unsigned char read_buf [buffer_size];
const int cb_size = buffer_size*12; // we want our circular buffer to be able to hold multiple messages
// Create a circular buffer with a capacity of cb_size
boost::circular_buffer<uint8_t> cb(cb_size);
serial_example::TactileData msg;
#define NUM_SENSOR_MIN 8
#define NUM_SENSOR_MAX 29
#define NUM_SENSORS 22



void parse_message(boost::circular_buffer<uint8_t> &cb, uint8_t start_index, uint8_t message_length, const std::array<ros::Publisher, NUM_SENSORS> &publishers)
{
    // populate the timestamp field
    uint32_t timestamp;
    timestamp = (cb[start_index+3] << 24) | (cb[start_index+4] << 16) | (cb[start_index+5] << 8) | cb[start_index+6];
    msg.timestamp = timestamp;

    // push the data into the data field
    uint16_t data_entry;
    for (size_t i=7; i < message_length; i++)
    {
        // if i is odd
        if ((i % 2) != 0)
        {
            //grab MSB of data for current taxel
            data_entry = (cb[start_index + i] << 8);
        }

        // if i is even
        else
        {
            // grab LSB of data for current taxel
            data_entry = data_entry | cb[start_index + i];
            // push byte into msg.data
            msg.data.push_back(data_entry);
        }
    }

    // publish the message to the appropriate topic
    uint8_t sensor_num = cb[start_index + 2];
    if ((sensor_num >= NUM_SENSOR_MIN) && (sensor_num <= NUM_SENSOR_MAX))
    {
        #ifdef PRINT
        printf("sensor_num= %i, message_length = %i\n", sensor_num, message_length);
        #endif
        publishers[sensor_num-NUM_SENSOR_MIN].publish(msg);
        msg.data.clear();
    }
    else
    {
        #ifdef PRINT
        printf("BAD SENSOR NUM: start_byte = %i, checksum = %i, sensor_num=: %i \n", cb[start_index], cb[start_index+1], sensor_num);
        #endif
    }
}

int main (int argc, char** argv)
{
    ros::init(argc, argv, "serial_example_node");
    ros::NodeHandle nh;

    /* Create Publishers */
    std::array<ros::Publisher, NUM_SENSORS> publishers;
    for (size_t i=0; i < NUM_SENSORS; i++)
    {
        publishers[i] = nh.advertise<serial_example::TactileData>("sensor_" + std::to_string(i+NUM_SENSOR_MIN), 1000);
    }

     /* Open and Configure Serial Port */
    // open the serial port
    int serial_port = open("/dev/ttyUSB0", O_RDONLY); //The serial port has to be properly renamed in certain cases
    
    // check for errors
    if (serial_port < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
    }

    // create struct for configuring the serial port
    struct termios tty;

    // read in existing settings and check for errors errors 
    if(tcgetattr(serial_port, &tty) != 0) 
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }

    // Set in/out baud rate to be 115200
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    // configure VMIN and VTIME -- no blocking
    tty.c_cc[VTIME] = 100;
    tty.c_cc[VMIN] = 0;

    // Setting other Port Stuff
    cfmakeraw(&tty);

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0)
    {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }

    ros::Rate loop_rate(5);

    /* Read and Parse Data */
    uint8_t byte_i; // the ith message of any given read 
    uint8_t message_length; // the checksum we received in the message, encoding the message length
    int start_index = 0;
    uint16_t end_index;

	cb[cb_size] =1;

    while(ros::ok())
    {

        ros::spinOnce();

        // read from USB into read_buf IF we're ready for new information
        if ((start_index > (cb_size - 2*buffer_size)) || !cb.full())
        {
            n = read(serial_port, &read_buf, sizeof(read_buf));
			std::cout << n << std::endl;
            // transfer data from read_buf into cb
            cb.insert(cb.end(), read_buf, read_buf + n);

            if (cb.full())
            {
                start_index -= n;

            }
        }


        while (cb[start_index] != 1)
        {
            start_index = (start_index + 1) % cb_size;
        }

        end_index = std::distance(cb.begin(), cb.end()) - 1; // this will just be cb_size after a few seconds...
        // as long as we have one more byte after the start byte
        if (start_index  < end_index) 
        {
            // then we can get the message length
            message_length = cb[start_index + 1]; 

            // THEN we can check if we've already received the full message
            if ((start_index + message_length - 1) <= end_index)
            {
                // and make sure that the end byte is valid
                if (cb[start_index + message_length - 1] == '\n')
                {
                    // populate the timestamp field
                    uint32_t timestamp;
                    timestamp = (cb[start_index+3] << 24) | (cb[start_index+4] << 16) | (cb[start_index+5] << 8) | cb[start_index+6];
                    msg.timestamp = timestamp;

                    // push the data into the data field
                    uint16_t data_entry;
                    for (size_t i=7; i < message_length; i++)
                    {
                        // if i is odd
                        if ((i % 2) != 0)
                        {
                            //grab MSB of data for current taxel
                            data_entry = (cb[start_index + i]);
                        }

                        // if i is even
                        else
                        {
                            // grab LSB of data for current taxel
                            data_entry = data_entry | (cb[start_index + i] << 8);
                            // push byte into msg.data
                            msg.data.push_back(data_entry);
                        }
                    }

                    // publish the message to the appropriate topic
                    uint8_t sensor_num = cb[start_index + 2];
                    if ((sensor_num >= NUM_SENSOR_MIN) && (sensor_num <= NUM_SENSOR_MAX))
                    {
                        #ifdef PRINT
                        printf("sensor_num= %i, message_length = %i\n", sensor_num, message_length);
                        #endif
                        publishers[sensor_num-NUM_SENSOR_MIN].publish(msg);
                        msg.data.clear();
                    }
                    else
                    {
                        #ifdef PRINT
                        printf("BAD SENSOR NUM: start_byte = %i, checksum = %i, sensor_num=: %i \n", cb[start_index], cb[start_index+1], sensor_num);
                        #endif
                    }

                    // parse_message(cb, start_index, message_length, publishers);
                }
                else
                {
                    #ifdef PRINT
                    printf("didn't find right end byte for sensor num = %i \n", cb[start_index + 1]);
                    #endif
                }
                start_index = (start_index + message_length) % cb_size;
            }
            else
            {
                #ifdef PRINT
                printf("didn't receive a full byte for sensor num = %i \n", cb[start_index + 1]);
                #endif
                continue;
            }

        }
        else
        {
            std::cout << "don't have the checksum yet \n";
        }

        }
    
    loop_rate.sleep();

    }
