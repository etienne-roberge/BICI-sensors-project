#!/usr/bin/env python3

import rospy
import matplotlib.pyplot as plt
from serial_example.msg import TactileData
import numpy as np

def callback(msg):
    tactile_data = np.reshape(np.asarray(msg.data), (2, 3))
    print(tactile_data)
    # im.set_array(tactile_data)
    # fig.canvas.draw()
    im.set_array(np.random.random((2, 3)))
    # redraw the figure
    fig.canvas.draw()

def listener():
    rospy.init_node('listener', anonymous=True)
    for sensor_num in [3]:
        rospy.Subscriber("sensor_" + str(sensor_num + 1), TactileData, callback)

if __name__ == '__main__':
    taxels = {1: (2,7), 'sape': 4139}
    var = input("Please enter a sensor number from 1-22: ")
    if var > 0 and var < 23:
        print("You entered: " + var)
    else:
        var = input("Come on fam, something from 1-22: ")
    fig = plt.figure()
    ax = fig.add_subplot(111)
    im = ax.imshow(np.random.random((2, 3)))
    plt.show()
    listener()
    rospy.spin()



