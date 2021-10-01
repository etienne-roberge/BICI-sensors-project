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
// #include <vector>

#define DEBUG

int n;// The number of bytes read
const int Buffer_size = 58; // largest possible buffer size
unsigned char read_buf [Buffer_size];
// std_msgs::UInt16MultiArray msg;
serial_example::TactileData msg;
#define NUM_SENSOR_MIN 8
#define NUM_SENSOR_MAX 30
#define NUM_SENSORS 22



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
    int serial_port = open("/dev/ttyUSB0", O_RDWR); //The serial port has to be properly renamed in certain cases
    
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
    tty.c_cc[VTIME] = 0;
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

    uint8_t readInLast = 0; // bytes from last read that are part of the same sensor message
    uint8_t chksum; // the checksum we received in the message
    uint8_t chksum_calc; // our own calculated checksum
    uint8_t sensor_num; // the sensor number we recieved in the message
    uint32_t timestamp; // the timestamp we recieved in the message
    uint8_t byte_i; // the ith message of any given read 
    uint16_t data_entry; // for storing data from one taxel, which is composed of two bytes
    int i_adj; // adjusted index which takes into account bytes from last read, if applicable

    while(ros::ok())
    {

        ros::spinOnce();

        n = read(serial_port, &read_buf, sizeof(read_buf));

        // loop through all the bytes we recieved
        for(int i = 0;i<n; i++) 
        {
            // grab the current byte
            byte_i = read_buf[i];
            #ifdef DEBUG
            printf("byte %i = %u \n", i, byte_i);
            #endif

            // incremement chksum_calc
            chksum_calc++;
            

            // i_adj = i + readInLast
            i_adj = i + readInLast;

            // if i_adj = 0
            if (i_adj == 0)
            {
                // check for valid start byte
                if (byte_i != 1)
                {
                    #ifndef DEBUG
                    std::cout << "Expected valid start byte but didn't get it.\n";
                    #endif
                    // i_adj = i_adj - 1; // keep looking for the start byte
                }
                else
                {
                    #ifndef DEBUG
                    // std::cout << "Got a valid start byte.\n";
                    #endif
                }

            }
    
            // else if i_adj = 1
            else if (i_adj == 1)
            {
                // save checksum for checking later
                chksum = byte_i;
            }
                
            // else if i_adj = 2
            else if (i_adj == 2)
            {
                // grab sensor number
                sensor_num = byte_i;
                #ifndef DEBUG
                printf("sensor number = %i \n ", sensor_num);
                #endif
            }
            // else if i_adj = 3
            else if (i_adj == 3)
            {             
                // grab MSB of timestamp
                timestamp = (byte_i << 24);   
            }
            // else if i_adj = 4
            else if (i_adj == 4)
            {
                // grab second MSB of timestamp
                timestamp = timestamp | (byte_i << 16);
            }
            // else if i_adj = 5
            else if (i_adj == 5)
            {
                // grab third MSB of timestamp
                timestamp = timestamp | (byte_i << 8);
            }
            // else if i_adj = 6
            else if (i_adj == 6)
            {
                // grab LSB of timestamp
                timestamp = timestamp | byte_i;
                // set timestamp field of msg
                msg.timestamp = timestamp;
            }

            // else if byte_i is not a valid end byte
            else if (byte_i != '\n') 
            {
                // if i_adj is odd
                if ((i_adj % 2) == 0)
                {
                    //grab MSB of data for current taxel
                    data_entry = (byte_i << 8);
                }
                    
                // else
                else
                {
                    // grab LSB of data for current taxel
                    data_entry = data_entry | byte_i;
                    // push byte into msg.data
                    msg.data.push_back(data_entry);
                }         
            }        
            
            // if this is the last byte we read
            if (i == (n-1))
            {
                // if it is a valid end byte
                if (byte_i == '\n')
                {
                    // check the checksum
                    if (chksum_calc == chksum)
                    {   
                        // we've finished a whole message!
                        #ifndef DEBUG
                        printf("Good checksum! We got i = %i \n ", chksum_calc);
                        #endif
                    }
                    else
                    {
                        #ifndef DEBUG
                        printf("Issue with checksum.... we got i = %i \n ", chksum_calc);
                        #endif
                    }
                    // regardless, reset the checksum calculation
                    chksum_calc = 0;
                    // 
                    readInLast = 0;
                    
                }
                // else
                else 
                {
                    // set readInLast so we can finish the message next time
                    readInLast = n;
                }
                    
            }

        }
        
        // if we've finished a message 
        if ((readInLast == 0) && (sensor_num >= NUM_SENSOR_MIN) && (sensor_num <= NUM_SENSOR_MAX))
        {
            publishers[sensor_num-NUM_SENSOR_MIN].publish(msg);
            msg.data.clear();
        }

        }
    
    loop_rate.sleep();

    }
