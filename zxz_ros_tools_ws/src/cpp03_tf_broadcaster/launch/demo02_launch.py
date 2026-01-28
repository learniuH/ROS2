from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python import get_package_share_directory
import os

def generate_launch_description():
    # rviz2文件默认位置
    package_path = get_package_share_directory('cpp03_tf_broadcaster')
    default_rviz_path = os.path.join(package_path, 'launch', 'dynamic_view.rviz')

    turtle = Node(package='turtlesim', executable='turtlesim_node')

    # 通过新终端启动键盘控制节点
    turtle_keyboard = Node(
        package='turtlesim', 
        executable='turtle_teleop_key', 
        prefix='gnome-terminal --', 
        output='screen'
    )

    rviz2 = Node(package='rviz2', executable='rviz2', arguments=['-d', default_rviz_path])

    dynamic_tf = Node(package='cpp03_tf_broadcaster', executable='demo02_dynamic_tf_broadcaster')

    return LaunchDescription([turtle, turtle_keyboard, rviz2, dynamic_tf])