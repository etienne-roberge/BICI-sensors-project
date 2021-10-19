bici_sensors

**ROS Communication Package**
In order to download the setup the packages needed to communicate with the Allegro Hand, please execute the following shell scripts in order and follow the instructions as prompted:
1- Pcan_Downloads
2- Pcan_Bus_Setup

In order to read data from the tactile sensors, the USB serial communication must be setup by executing the following shell script:
serial_USB_Setup

After building everything in ROS_COMM_WS using catkin, the following command will initiate the torque control of the Allegro hand, as well as contact visualization/recording:
roslaunch allegro_hand_controllers allegro_hand.launch HAND:=right CONTROLLER:=torque VISUALIZE:=true

**P.S.** When building the ROS package, "serial_comm" package must be built first using the command: catkin_make --only-pkg-with-deps <target_package> , then switching back to building all the packages is needed using this command: catkin_make -DCATKIN_WHITELIST_PACKAGES=""
