from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    # 1.启动turtlesim_node
    turtle = Node(package='turtlesim', executable='turtlesim_node')

    # 2.自定义的服务端
    exer_server = Node(package='cpp08_service_exercise', executable='exer01_server')

    return LaunchDescription([turtle, exer_server])