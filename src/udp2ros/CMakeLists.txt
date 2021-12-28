cmake_minimum_required(VERSION 3.0.2)
project(udp2ros)

add_compile_options(-std=c++14)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  message_generation
)

 add_message_files(
   FILES
   UdpPacket.msg
 )

 generate_messages(
   DEPENDENCIES
   std_msgs  # Or other packages containing msgs
 )

catkin_package(
#  INCLUDE_DIRS include
  LIBRARIES udp2ros
  CATKIN_DEPENDS roscpp message_runtime
#  DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

 add_executable(udp_receive ${PROJECT_SOURCE_DIR}/src/udp_receive_ros1.cpp)
 add_executable(udp_send ${PROJECT_SOURCE_DIR}/src/udp_send_ros1.cpp)
 add_executable(udp_send_receive ${PROJECT_SOURCE_DIR}/src/udp_send_receive_test_ros1.cpp)

 target_link_libraries(udp_receive ${catkin_LIBRARIES})
 target_link_libraries(udp_send ${catkin_LIBRARIES})
 target_link_libraries(udp_send_receive ${catkin_LIBRARIES})

