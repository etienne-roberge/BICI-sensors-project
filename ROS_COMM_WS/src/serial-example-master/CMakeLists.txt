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


link_directories(/usr/local/lib)
find_package(PkgConfig REQUIRED)
include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(serial_example_node src/serial_example_node.cpp)
add_executable(sensor_parser src/sensor_parser.cpp)
add_dependencies(serial_example_node serial_example_generate_messages_cpp )

target_link_libraries(serial_example_node
  ${catkin_LIBRARIES}
)
add_dependencies(serial_example_node serial_example_generate_messages_cpp)

target_link_libraries(sensor_parser
  ${catkin_LIBRARIES}
)


