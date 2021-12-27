
#include <ros/ros.h>
int main(int argc, char **argv) {
  ros::init(argc, argv, "test");
  
  ros::NodeHandle n;
  ROS_INFO("---TEST---");
  ros::spin();
  
  return 0;
}

