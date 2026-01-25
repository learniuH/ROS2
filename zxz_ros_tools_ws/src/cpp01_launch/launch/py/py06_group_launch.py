from launch import LaunchDescription
from launch_ros.actions import Node, PushRosNamespace
from launch.actions import GroupAction

def generate_launch_description():
    turtle1 = Node(package='turtlesim', executable='turtlesim_node', name='turtle1')
    turtle2 = Node(package='turtlesim', executable='turtlesim_node', name='turtle2')
    turtle3 = Node(package='turtlesim', executable='turtlesim_node', name='turtle3')

    # 分组：设置当前组的命名空间，以及包含的节点
    group1 = GroupAction(actions=[PushRosNamespace(namespace='group1'), turtle1, turtle2])
    group2 = GroupAction(actions=[PushRosNamespace(namespace='group2'), turtle3])

    return LaunchDescription([group1, group2])