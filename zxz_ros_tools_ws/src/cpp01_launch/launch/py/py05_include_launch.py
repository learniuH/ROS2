from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource

from ament_index_python.packages import get_package_share_directory
import os

"""
需求: 在当前launch文件中包含其他launch文件
"""

other_launch_path = os.path.join(
    get_package_share_directory('cpp01_launch'),
    'launch/py',
    'py04_args_launch.py'
)

def generate_launch_description():
    include = IncludeLaunchDescription(
        launch_description_source=PythonLaunchDescriptionSource(
            launch_file_path=other_launch_path
        ),
        launch_arguments=[
            ('launch_arg_bgr', '255'),
            ('launch_arg_bgg', '86'),
            ('launch_arg_bgb', '255')
        ]
        # launch_arguments={
        #     'launch_arg_bgr': '255',
        #     'launch_arg_bgg': '86',
        #     'launch_arg_bgb': '255'
        # }.items()
    )

    return LaunchDescription([include])