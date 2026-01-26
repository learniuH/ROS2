# ros2 run 修改话题名称

***普通turtlesim_node节点***
```bash
$ ros2 run turtlesim turtlesim_node

$ ros2 node list
/turtlesim

$ ros2 topic list 
/turtle1/cmd_vel
/turtle1/color_sensor
/turtle1/pose
```

1. ros2 run 设置命名空间
```bash
$ ros2 run turtlesim turtlesim_node --ros-args --remap __ns:=/t1

$ ros2 node list 
/t1/turtlesim

$ ros2 topic list 
/t1/turtle1/cmd_vel
/t1/turtle1/colcor_sensor
/t1/turtle1/pose
```

2. ros2 run 设置话题重映射
```bash
$ ros2 run turtlesim turtlesim_node --ros-args --remap /turtle1/cmd_vel:=/cmd_vel

$ ros2 node list 
/turtlesim

$ ros2 topic list 
/cmd_vel
/turtle1/colcor_sensor
/turtle1/pose
```