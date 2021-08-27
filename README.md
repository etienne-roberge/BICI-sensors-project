bici_sensors

**ROS Communication Package**

1- Use the following command: dmesg | grep tty  to identify the name of the USB port

2- Change the permission to the USB port already found in the previous step, using the following command: sudo chmod 777 /dev/”USB Name”

3- Configure the parameters of the USB serial port communication to match the parameters used while programming the chip using psoc creator, using the following command: stty -F /dev/”USB Name” 9600 cs8 -cstopb -parenb

4- Use the ros node that you’ve built to publish the data coming from the serial usb port into a rostopic, using the following command: rosrun serial_example serial_example_node

**P.S.**  After activating the previously mentioned ros node, the data get published on the topic /read
