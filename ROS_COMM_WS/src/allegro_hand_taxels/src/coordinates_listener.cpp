#include <ros/ros.h>
#include <visualization_msgs/MarkerArray.h>
#include <visualization_msgs/Marker.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>
#include <serial_comm/TactileData.h>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <cstring>
#include <std_msgs/String.h>
#define NUM_SENSORS 22
#define NUM_SENSOR_MIN 8
#define PTS_NUM 400
std::ofstream myfile;
using namespace std;
tf2_ros::Buffer tfBuffer;
tf2_ros::TransformListener *tfListener;
geometry_msgs::TransformStamped taxel_1_trans, taxel_2_trans;
float taxel_1_x, taxel_1_y, taxel_1_z, taxel_2_x, taxel_2_y, taxel_2_z;
/*
class Tactile_Sensor {
  public:
    uint8_t sensor_num;        // ID of the sensor
    int size;                  // The number of taxels
    string link_name;          // The link on which the sensor is installed
    uint16_t threshold = 1000;  // Capacitance raw count threshold to detect contact.
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
Tactile_Sensor sensor22(29, 6, "link_6_front_upper");
Tactile_Sensor sensor21(28, 20, "link_7_wedge_tip");
Tactile_Sensor sensor20(27, 15, "link_1_back");
Tactile_Sensor sensor19(26, 6, "link_2_back");
Tactile_Sensor sensor18(25, 6, "link_1_front_lower");
Tactile_Sensor sensor17(24, 6, "link_2_front_upper");
Tactile_Sensor sensor16(23, 20, "link_3_wedge_tip");
Tactile_Sensor sensor15(22, 25, "link_palm_back");
Tactile_Sensor sensor14(21, 20, "link_palm_front");
Tactile_Sensor sensor13(20, 6, "link_14_back_lower");
Tactile_Sensor sensor12(19, 6, "link_15_back_upper");
Tactile_Sensor sensor11(18, 6, "link_14_front_lower");
Tactile_Sensor sensor10(17, 6, "link_15_front_upper");
Tactile_Sensor sensor9(16, 20, "link_15_wedge_tip");
Tactile_Sensor sensor8(15, 15, "link_9_back");
Tactile_Sensor sensor7(14, 6, "link_10_back");
Tactile_Sensor sensor6(13, 6, "link_9_front_lower");
Tactile_Sensor sensor5(12, 6, "link_10_front_upper");
Tactile_Sensor sensor4(11, 20, "link_11_wedge_tip");
Tactile_Sensor sensor3(10, 15, "link_5_back");
Tactile_Sensor sensor2(9, 6, "link_6_back");
Tactile_Sensor sensor1(8, 6, "link_5_front_lower");

Tactile_Sensor Sensors_Array[NUM_SENSORS]{sensor1, sensor2, sensor3, sensor4, sensor5, sensor6, sensor7, sensor8,
sensor9, sensor10, sensor11, sensor12, sensor13, sensor14, sensor15, sensor16, sensor17, sensor18, sensor19,
sensor20, sensor21, sensor22};
vector<vector<float>> points_storage; //Container for the contact points to be stored
string act_cmd = "false";
tf2_ros::Buffer tfBuffer;
tf2_ros::TransformListener *tfListener;
map <string, visualization_msgs::Marker> markers_map; // A dictionary for the markers placed on the taxels
visualization_msgs::MarkerArray markers_array; //Array of markers (e.g. a marker is created for each taxel)
//One common callback function
void callback(const serial_comm::TactileData msg){
    Sensors_Array[msg.sensor_num-8].timestamp = msg.timestamp;
    Sensors_Array[msg.sensor_num-8].data = msg.data;
    //cout<<act_cmd<<endl;
    if (act_cmd == "true") // Activation of a save command through a ros node
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
     
    for(int taxel=0;taxel<Sensors_Array[msg.sensor_num-8].size;taxel++){
       if (Sensors_Array[msg.sensor_num-8].data[taxel]<= Sensors_Array[msg.sensor_num-8].threshold)
       {
           //cout << "marker color should change" << endl;
            visualization_msgs::Marker marker;
            marker.ns = Sensors_Array[msg.sensor_num-8].link_name+"_taxel";
            marker.id= taxel;
            marker.header.frame_id = Sensors_Array[msg.sensor_num-8].link_name+"_taxel_"+to_string(taxel);
            marker.header.stamp = ros::Time(0);
            marker.type = visualization_msgs::Marker::SPHERE;
            marker.action = visualization_msgs::Marker::ADD;
            marker.pose.position.x = 0;
            marker.pose.position.y = 0;
            marker.pose.position.z = 0;
            marker.pose.orientation.x = 0.0;
            marker.pose.orientation.y = 0.0;
            marker.pose.orientation.z = 0.0;
            marker.pose.orientation.w = 1.0;
            marker.scale.x = 0.012;
            marker.scale.y = 0.012;
            marker.scale.z = 0.012;
            marker.color.r = 1.0f;
            marker.color.g = 0.2f;
            marker.color.b = 0.2f;
            marker.color.a = 1.0;
            marker.lifetime = ros::Duration();
            markers_array.markers.push_back(marker);
            
        }
        else{
            //cout << "marker color should come back to normal" << endl;
            visualization_msgs::Marker marker;
            marker.ns = Sensors_Array[msg.sensor_num-8].link_name+"_taxel";
            marker.id= taxel;
            marker.header.frame_id = Sensors_Array[msg.sensor_num-8].link_name+"_taxel_"+to_string(taxel);
            marker.header.stamp = ros::Time(0);
            marker.type = visualization_msgs::Marker::SPHERE;
            marker.action = visualization_msgs::Marker::ADD;
            marker.pose.position.x = 0;
            marker.pose.position.y = 0;
            marker.pose.position.z = 0;
            marker.pose.orientation.x = 0.0;
            marker.pose.orientation.y = 0.0;
            marker.pose.orientation.z = 0.0;
            marker.pose.orientation.w = 1.0;
            marker.scale.x = 0.012;
            marker.scale.y = 0.012;
            marker.scale.z = 0.012;
            marker.color.r = 1.0f;
            marker.color.g = 0.2f;
            marker.color.b = 0.2f;
            marker.color.a = 0.0;
            marker.lifetime = ros::Duration();
            markers_array.markers.push_back(marker);
             
        }
       
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
*/

int main(int argc, char** argv){
    ros::init(argc, argv, "my_tf2_listener");
    ros::NodeHandle node;
    ros::Rate rate(1.0);
    tfListener = new tf2_ros::TransformListener(tfBuffer);
    /*std::array<ros::Subscriber, NUM_SENSORS> subscribers;
    myfile.open ("points_coordinates.csv");
    ros::Subscriber saving_subscriber; //A subscriber to the saving_activation command
    for (size_t i=0; i < subscribers.size(); i++)
    {
        subscribers[i] = node.subscribe<serial_comm::TactileData>("sensor_" + std::to_string(i+NUM_SENSOR_MIN)+"_readings", 5, callback);
    }
    saving_subscriber = node.subscribe<std_msgs::String>("saving_command", 5, saving_callback);
    // Publisher for the rviz markers
    ros::Publisher markers_pub = node.advertise<visualization_msgs::MarkerArray>("visualization_markers", 1000);

    while(ros::ok()){

    markers_pub.publish(markers_array);
    ros::spinOnce();
    rate.sleep();
    }*/
    while(ros::ok()){
            if ((tfBuffer.canTransform("taxel_1","palm_link", ros::Time(0))) && (tfBuffer.canTransform("taxel_2","palm_link", ros::Time(0)))) 
       {
           try{
               taxel_1_trans = tfBuffer.lookupTransform("palm_link", "taxel_1", ros::Time(0));   
               taxel_2_trans = tfBuffer.lookupTransform("palm_link", "taxel_2", ros::Time(0));                                                                                                                                               
           }
           catch (ros::Exception &ex) {
            ROS_WARN("%s",ex.what());
            ros::Duration(1.0).sleep();
            continue;
            }
            //X axis coordinates
            taxel_1_x = taxel_1_trans.transform.translation.x;
            cout<< "Taxel 1 X Coordinate: " << taxel_1_x;
            taxel_2_x = taxel_2_trans.transform.translation.x;
            cout<< " ............ Taxel 2 X Coordinate: " << taxel_2_x << endl;
            //Y axis coordinates  
            taxel_1_y = taxel_1_trans.transform.translation.y;
            cout<< "Taxel 1 Y Coordinate: " << taxel_1_y;
            taxel_2_y = taxel_2_trans.transform.translation.y;
            cout<< " ............ Taxel 2 Y Coordinate: " << taxel_2_y << endl;
            //Z axis coordinates  
            taxel_1_z = taxel_1_trans.transform.translation.z;
            cout<< "Taxel 1 Z Coordinate: " << taxel_1_z;
            taxel_2_z = taxel_2_trans.transform.translation.z;
            cout<< " ............ Taxel 2 Z Coordinate: " << taxel_2_z << endl;




       }
       rate.sleep();

    }


        return 0;
        };