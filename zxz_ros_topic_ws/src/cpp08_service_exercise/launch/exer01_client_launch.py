from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess

import random

def generate_launch_description():
    # turtlesim_node坐标系：以左下角为原点，向右为x轴正方向[0, 11]，向上为y轴正方向[0, 11]
    x = random.uniform(2, 10)
    y = random.uniform(2, 10)
    theta = random.uniform(0, 6.28)
    name = 'turtle2'
    
    # 1.在随即位置生成乌龟
    spawn_turtle2 = ExecuteProcess(
        cmd=[f'ros2 service call /spawn turtlesim/srv/Spawn "{{\'x\': {x}, \'y\': {y}, \'theta\': {theta}, \'name\': {name}}}"'],
        output='both',
        shell=True
    )

    # 2.启动客户端并发送坐标信息
    exer_client = Node(
        package='cpp08_service_exercise', 
        executable='exer01_client',
        arguments=[str(x), str(y), str(theta)]  # ros2 run ... x y theta --ros-args
    )

    return LaunchDescription([spawn_turtle2, exer_client])