from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python import get_package_share_directory
import os

def generate_launch_description():
    static_tf_launch_path = os.path.join(
        get_package_share_directory('cpp03_tf_broadcaster'),
        'launch',
        'demo01_launch.py'
    )

    launch_static_tf = IncludeLaunchDescription(
        launch_description_source=PythonLaunchDescriptionSource(
            launch_file_path=static_tf_launch_path
        )
    )

    point_tf = Node(package='cpp03_tf_broadcaster', executable='demo03_point_tf_broadcaster')

    return LaunchDescription([launch_static_tf, point_tf])