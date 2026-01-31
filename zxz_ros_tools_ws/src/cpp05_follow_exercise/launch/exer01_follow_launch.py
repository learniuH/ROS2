from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from ament_index_python import get_package_share_directory

import os
import random

def generate_launch_description():
    rviz_path = os.path.join(
        get_package_share_directory('cpp05_follow_exercise'),
        'launch',
        'exer01_follow.rviz'
    )

    x = random.uniform(1.0, 10.0)
    y = random.uniform(1.0, 10.0)
    theta = random.uniform(0.0, 6.28)
    turtle_name = DeclareLaunchArgument(name="turtle_name", default_value="turtle2")

    turtle = Node(package='turtlesim', executable='turtlesim_node')

    spawn = Node(
        package='cpp05_follow_exercise',
        executable='exer01_spawn',
        parameters=[
            {'x': x}, 
            {'y': y}, 
            {'theta': theta},
            {'name': LaunchConfiguration('turtle_name')}
        ]
    )

    tf_broadcaster_1 = Node(
        package='cpp05_follow_exercise',
        executable='exer01_transform_broadcaster',
        name='tf_broadcaster_1',
    )

    tf_broadcaster_2 = Node(
        package='cpp05_follow_exercise',
        executable='exer01_transform_broadcaster',
        name='tf_broadcaster_2',
        parameters=[{'turtle_name': LaunchConfiguration('turtle_name')}]
    )

    cmd_publisher = Node(
        package='cpp05_follow_exercise',
        executable='exer01_transform_listener',
        parameters=[
            {'frame_id': LaunchConfiguration('turtle_name')},
            {'child_frame_id': "turtle1"}
        ]
    )

    turtle_keyboard = Node(
        package='turtlesim',
        executable='turtle_teleop_key',
        prefix='gnome-terminal --',
        output='screen'
    )

    rviz2 = Node(
        package='rviz2',
        executable='rviz2',
        arguments=['-d', rviz_path]
    )

    return LaunchDescription([turtle_name, turtle, spawn, tf_broadcaster_1, tf_broadcaster_2, cmd_publisher, turtle_keyboard, rviz2])