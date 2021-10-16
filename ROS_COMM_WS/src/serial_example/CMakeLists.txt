cmake_minimum_required(VERSION 2.8.3)
project(serial_example)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  message_generation
)

add_message_files(
  FILES
  TactileData.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
  CATKIN_DEPENDS
    std_msgs
    message_runtime
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(sensor_parser src/sensor_parser.cpp)

target_link_libraries(sensor_parser
  ${catkin_LIBRARIES}
)


