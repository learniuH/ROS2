# ros2 run 设置节点名称

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
/t1/turtle1/color_sensor
/t1/turtle1/pose
```

2. ros2 run 设置节点重映射
```bash
$ ros2 run turtlesim turtlesim_node --ros-args --remap __node:=turtle1
$ ros2 run turtlesim turtlesim_node --ros-args --remap __name:=turtle2

$ ros2 node list 
/turtle1
/turtle2

$ ros2 topic list 
/turtle1/cmd_vel
/turtle1/color_sensor
/turtle1/pose
```

3. ros2 run 设置命名空间与名称重映射叠加
```bash
$ ros2 run turtlesim turtlesim_node --ros-args --remap __ns:=/t1 --remap __name:=cmd_turtle

$ ros2 node list
/t1/turtle1

$ ros2 topic list 
/t1/turtle1/cmd_vel
/t1/turtle1/color_sensor
/t1/turtle1/pose
```