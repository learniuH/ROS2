# rosbag2相关
1. 录制bag包
```bash
$ ros2 bag record /turtle1/cmd_vel -o ROS2/zxz_ros_tools_ws/src/cpp02_rosbag/bag_cmd/
```
2. 回放bag包
```bash
$ ros2 bag play ROS2/zxz_ros_tools_ws/src/cpp02_rosbag/bag_cmd/
```