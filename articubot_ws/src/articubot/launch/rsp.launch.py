from launch import LaunchDescription
from launch_ros.actions import Node
from launch_ros.descriptions import ParameterValue
from launch.substitutions import Command
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    package_path = get_package_share_directory('articubot')
    default_description_file = os.path.join(
        package_path, 'description', 'robot.urdf.xacro'
    )
    default_rviz_path = os.path.join(
        package_path, 'config', 'view_bot.rviz'
    )

    robot_description = ParameterValue(Command(['xacro ', default_description_file]))

    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'robot_description': robot_description}]
    )

    joint_state_publisher_gui = Node(
        package='joint_state_publisher_gui',
        executable='joint_state_publisher_gui'
    )

    rviz2 = Node(
        package='rviz2',
        executable='rviz2',
        arguments=['-d', default_rviz_path]
    )

    return LaunchDescription([robot_state_publisher, joint_state_publisher_gui, rviz2])