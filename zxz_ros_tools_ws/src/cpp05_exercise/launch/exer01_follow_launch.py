from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python import get_package_share_directory

import os
import random

def generate_launch_description():
    rviz_path = os.path.join(
        get_package_share_directory('cpp05_exercise'),
        'launch',
        'exer01_follow.rviz'
    )

    x = random.uniform(1.0, 10.0)
    y = random.uniform(1.0, 10.0)
    theta = random.uniform(0.0, 6.28)
    name = 'turtle2'
    
    turtle = Node(package='turtlesim', executable='turtlesim_node')

    spawn = Node(
        package='cpp05_exercise',
        executable='exer01_spawn',
        parameters=[
            {'x': x}, 
            {'y': y}, 
            {'theta': theta},
            {'name': name}
        ]
    )

    tf_broadcaster_1 = Node(
        package='cpp05_exercise',
        executable='exer01_transform_broadcaster',
        name='tf_broadcaster_1',
    )

    tf_broadcaster_2 = Node(
        package='cpp05_exercise',
        executable='exer01_transform_broadcaster',
        name='tf_broadcaster_2',
        parameters=[{'turtle_name': name}]
    )

    rviz2 = Node(
        package='rviz2',
        executable='rviz2',
        arguments=['-d', rviz_path]
    )

    return LaunchDescription([turtle, spawn, tf_broadcaster_1, tf_broadcaster_2, rviz2])