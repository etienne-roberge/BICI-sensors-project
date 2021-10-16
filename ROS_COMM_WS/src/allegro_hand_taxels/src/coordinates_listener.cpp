#include <ros/ros.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>
#include <serial_example/TactileData.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>
#include <std_msgs/String.h>
#define NUM_SENSORS 22
#define PTS_NUM 100
std::ofstream myfile;
using namespace std;

class Tactile_Sensor {
  public:
    uint8_t sensor_num;        // ID of the sensor
    int size;                  // The number of taxels
    string link_name;          // The link on which the sensor is installed
    uint16_t threshold = 500;  // Capacitance raw count threshold to detect contact.
                               // This might be later on initialized individually for each
                               //sensor or even taxel, after tuning
    uint32_t timestamp = 0; // This can be assigned to any random number
    std::vector<uint16_t> data;
    std::vector<geometry_msgs::TransformStamped> transformStamped_taxels;
    std::vector<std::vector<float>> taxels_coordinates;
    Tactile_Sensor(uint8_t sensor_num, int size, string link_name){
        //Any other number can be used for initialization
        this->sensor_num = sensor_num;
        this->size = size;
        this->link_name = link_name;
        this->data.resize(size);
        this->transformStamped_taxels.resize(size);
        this->taxels_coordinates.resize(size, std::vector<float>(3, 0));//Vector monitoring the sensor's taxels positions

    }
};
//The set of integrated sensors:
Tactile_Sensor sensor1(29, 6, "link_6_front_upper");
Tactile_Sensor sensor2(28, 14, "link_7_wedge_tip");
Tactile_Sensor sensor3(27, 15, "link_1_back");
Tactile_Sensor sensor4(26, 6, "link_2_back");

Tactile_Sensor sensor5(25, 6, "link_1_front_lower");
Tactile_Sensor sensor6(24, 6, "link_2_front_upper");
Tactile_Sensor sensor7(23, 14, "link_3_wedge_tip");
Tactile_Sensor sensor8(22, 25, "link_palm_back");
Tactile_Sensor sensor9(21, 20, "link_palm_front");
Tactile_Sensor sensor10(20, 6, "link_14_back_lower");
Tactile_Sensor sensor11(19, 6, "link_15_back_upper");
Tactile_Sensor sensor12(18, 6, "link_14_front_lower");
Tactile_Sensor sensor13(17, 6, "link_15_front_upper");
Tactile_Sensor sensor14(16, 14, "link_15__wedge_tip");
Tactile_Sensor sensor15(15, 15, "link_9_back");
Tactile_Sensor sensor16(14, 6, "link_10_back");
Tactile_Sensor sensor17(13, 6, "link_9_front_lower");
Tactile_Sensor sensor18(12, 6, "link_10_front_upper");
Tactile_Sensor sensor19(11, 14, "link_5_front_lower");
Tactile_Sensor sensor20(10, 15, "link_5_back");
Tactile_Sensor sensor21(9, 6, "link_6_back");
Tactile_Sensor sensor22(8, 6, "link_5_front_lower");

Tactile_Sensor Sensors_Array[NUM_SENSORS]{sensor1, sensor2, sensor3, sensor4, sensor5, sensor6, sensor7, sensor8,
sensor9, sensor10, sensor11, sensor12, sensor13, sensor14, sensor15, sensor16, sensor17, sensor18, sensor19,
sensor20, sensor21, sensor22};
vector<vector<float>> points_storage; //Container for the contact points to be stored
string act_cmd = "false";
tf2_ros::Buffer tfBuffer;
tf2_ros::TransformListener *tfListener;
//One common callback function
void callback(const serial_example::TactileData msg){
    Sensors_Array[msg.sensor_num-8].timestamp = msg.timestamp;
    Sensors_Array[msg.sensor_num-8].data = msg.data;
    //cout<<act_cmd<<endl;
    if (act_cmd == "true") // Activation of a save command through the terminal
    {
        for(int j=0;j<NUM_SENSORS;j++){
            //cout << Sensors_Array[j].taxels_coordinates.size() << endl;
            //cout << Sensors_Array[j].taxels_coordinates[0].size() << endl;
            for(int taxel=0;taxel<Sensors_Array[j].taxels_coordinates.size();taxel++){
                points_storage.push_back(Sensors_Array[j].taxels_coordinates[taxel]);
                //cout<< "X_Storage"<< points_storage[taxel][0] << endl;
                //cout<< "Y_Storage"<< points_storage[taxel][1] << endl;
                //cout<< "Z_Storage"<< points_storage[taxel][2] << endl;
            }
        }
        //Writing data to a csv file
        for (size_t i = 0; i < points_storage.size(); i++)
        {
            for (size_t j = 0; j < points_storage[i].size(); j++)
            {
                cout<< "Coordinates_Storage"<< points_storage[i][j] << endl;
                myfile << points_storage[i][j] << ',';
                //cout << "Recorded coordinate" << points_storage[i][j] << endl;
            }
            myfile << '\n';
            
        }
        myfile.close();
        act_cmd = "false";
        points_storage.clear();
        ros::shutdown();

    }
     
    /*for(int taxel=0;taxel<Sensors_Array[msg.sensor_num-8].size;taxel++){
       if (Sensors_Array[msg.sensor_num-8].data[taxel]<= Sensors_Array[msg.sensor_num-8].threshold)
       {
           try{
               Sensors_Array[msg.sensor_num-8].transformStamped_taxels[taxel] = tfBuffer.lookupTransform(Sensors_Array[msg.sensor_num-8].link_name+"_taxel_"+std::to_string(taxel),"palm_link", ros::Time(0));                                                                                                                                                    
           }
           catch (tf2::TransformException &ex) {
            ROS_WARN("%s",ex.what());
            ros::Duration(1.0).sleep();
            continue;
            }
            Sensors_Array[msg.sensor_num-8].taxels_coordinates[taxel][0] = Sensors_Array[msg.sensor_num-8].transformStamped_taxels[taxel].transform.translation.x;
            Sensors_Array[msg.sensor_num-8].taxels_coordinates[taxel][1] = Sensors_Array[msg.sensor_num-8].transformStamped_taxels[taxel].transform.translation.y;
            Sensors_Array[msg.sensor_num-8].taxels_coordinates[taxel][2] = Sensors_Array[msg.sensor_num-8].transformStamped_taxels[taxel].transform.translation.z;
       }
        
    }*/
    // Modified copy of the previous code block
    for(int taxel=0;taxel<Sensors_Array[msg.sensor_num-8].size;taxel++){
       if ((Sensors_Array[msg.sensor_num-8].data[taxel]<= Sensors_Array[msg.sensor_num-8].threshold) && tfBuffer.canTransform(Sensors_Array[msg.sensor_num-8].link_name+"_taxel_"+std::to_string(taxel),"palm_link", ros::Time(0))) 
       {
           //cout<< tfBuffer.canTransform(Sensors_Array[msg.sensor_num-8].link_name+"_taxel_"+std::to_string(taxel),"palm_link", ros::Time(0))<<endl;
           try{
               Sensors_Array[msg.sensor_num-8].transformStamped_taxels[taxel] = tfBuffer.lookupTransform(Sensors_Array[msg.sensor_num-8].link_name+"_taxel_"+std::to_string(taxel),"palm_link", ros::Time(0));                                                                                                                                                    
           }
           catch (ros::Exception &ex) {
            ROS_WARN("%s",ex.what());
            ros::Duration(1.0).sleep();
            continue;
            }
            Sensors_Array[msg.sensor_num-8].taxels_coordinates[taxel][0] = Sensors_Array[msg.sensor_num-8].transformStamped_taxels[taxel].transform.translation.x;
            //cout<< "X Coordinate" << Sensors_Array[msg.sensor_num-8].taxels_coordinates[taxel][0] << endl; 
            Sensors_Array[msg.sensor_num-8].taxels_coordinates[taxel][1] = Sensors_Array[msg.sensor_num-8].transformStamped_taxels[taxel].transform.translation.y;
            //cout<< "Y Coordinate" << Sensors_Array[msg.sensor_num-8].taxels_coordinates[taxel][1] << endl;
            Sensors_Array[msg.sensor_num-8].taxels_coordinates[taxel][2] = Sensors_Array[msg.sensor_num-8].transformStamped_taxels[taxel].transform.translation.z;
            //cout<< "Z Coordinate" << Sensors_Array[msg.sensor_num-8].taxels_coordinates[taxel][2] << endl;
       }
        
    }


}
void saving_callback(const std_msgs::String msg){
    act_cmd = msg.data;
}


int main(int argc, char** argv){
    ros::init(argc, argv, "my_tf2_listener");
    ros::NodeHandle node;
    ros::Rate rate(1.0);
    tfListener = new tf2_ros::TransformListener(tfBuffer);
    Sensors_Array[0].sensor_num = 1;
    Sensors_Array[0].size = 1;
    Sensors_Array[0].link_name = "link_3_tip";
    Sensors_Array[1].sensor_num = 2;
    Sensors_Array[1].size = 1;
    Sensors_Array[1].link_name = "link_7_tip";
    Sensors_Array[2].sensor_num = 3;
    Sensors_Array[2].size = 1;
    Sensors_Array[2].link_name = "link_11_tip";
    Sensors_Array[3].sensor_num = 4;
    Sensors_Array[3].size = 1;
    Sensors_Array[3].link_name = "link_15_tip";
    std::array<ros::Subscriber, NUM_SENSORS> subscribers;
    myfile.open ("points_coordinates.csv");
    ros::Subscriber saving_subscriber; //A subscriber to the saving_activation command
    for (size_t i=0; i < subscribers.size(); i++)
    {
        subscribers[i] = node.subscribe<serial_example::TactileData>("sensor_" + std::to_string(i+1)+"_readings", 5, callback);
    }
    saving_subscriber = node.subscribe<std_msgs::String>("saving_command", 5, saving_callback);
    
    /*while (node.ok()){
        try{
            Sensors_Array[0].transformStamped_taxels.push_back(tfBuffer.lookupTransform("link_3_tip_taxel_0","palm_link", ros::Time(0)));
            Sensors_Array[1].transformStamped_taxels.push_back(tfBuffer.lookupTransform("link_7_tip_taxel_0","palm_link", ros::Time(0)));
            Sensors_Array[2].transformStamped_taxels.push_back(tfBuffer.lookupTransform("link_11_tip_taxel_0","palm_link", ros::Time(0)));
            Sensors_Array[3].transformStamped_taxels.push_back(tfBuffer.lookupTransform("link_15_tip_taxel_0","palm_link", ros::Time(0)));
            }
        catch (tf2::TransformException &ex) {
            ROS_WARN("%s",ex.what());
            ros::Duration(1.0).sleep();
            continue;
            }
        for(int i=0;i<NUM_SENSORS;i++){
            for(int taxel=0;taxel<Sensors_Array[i].size;taxel++){
                Sensors_Array[i].taxels_x[taxel] = Sensors_Array[i].transformStamped_taxels[taxel].transform.translation.x;
                Sensors_Array[i].taxels_y[taxel] = Sensors_Array[i].transformStamped_taxels[taxel].transform.translation.y;
                Sensors_Array[i].taxels_z[taxel] = Sensors_Array[i].transformStamped_taxels[taxel].transform.translation.z;
            }
        }
        Printing the results
        for(int j=0;j<NUM_SENSORS;j++){
            for(int taxel=0;taxel<Sensors_Array[j].size;taxel++){
                cout<< Sensors_Array[j].name << " X coordinate:" << Sensors_Array[j].taxels_coordinates[taxel][0] << endl;
                cout << Sensors_Array[j].name << " Y coordinate:" << Sensors_Array[j].taxels_coordinates[taxel][1] << endl;
                cout << Sensors_Array[j].name << " Z coordinate:" << Sensors_Array[j].taxels_coordinates[taxel][2] << endl;
            }
        }
        // Storing the coordinates in ROS cutsom message
        for(int j=0;j<NUM_SENSORS;j++){
            msg.taxels_x = Sensors_Array[j].taxels_x;
            msg.taxels_y = Sensors_Array[j].taxels_y;
            msg.taxels_z = Sensors_Array[j].taxels_z;
        }*/

        ros::spin();
        return 0;
        };