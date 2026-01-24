from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    turtle = Node(package='turtlesim', executable='turtlesim_node')

    rgb_client = Node(package='cpp10_param_exercise', executable='exer01_param_client')
    
    return LaunchDescription([turtle, rgb_client])