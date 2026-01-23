from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    turtle = Node(package='turtlesim', executable='turtlesim_node')
    nav_server = Node(package='cpp09_action_exercise', executable='exer01_action_server')

    return LaunchDescription([turtle, nav_server])