from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():

    return LaunchDescription([
        # 通过launch.py修改节点名称，解决节点重名问题
        Node(package='turtlesim', executable='turtlesim_node', name='turtle_py'),
        Node(package='turtlesim', executable='turtlesim_node', namespace='ns_py'),
        Node(package='turtlesim', executable='turtlesim_node', namespace='ns_py', name='turtle_py'),
    ])