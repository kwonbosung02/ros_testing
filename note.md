### Workspace
```
source /opt/ros/noetic/setub.bash

mkdir -p ~/catkin_ws/src

cd ~catkin_ws

catkin_make

source devel/setup.bash
```
### Package
```
cd ~/catkin_ws/src

catkin_create_pkg package_nake std_msgs roscpp
```

### Build
```
cd ~/catkin_ws

catkin_make

. ~/catkin_ws/devel/setup.bash
```
