from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    t1 = Node(package='turtlesim', executable='turtlesim_node', name='py_t1')
    t2 = Node(package='turtlesim', executable='turtlesim_node', name='py_t2')

    return LaunchDescription([t1, t2])