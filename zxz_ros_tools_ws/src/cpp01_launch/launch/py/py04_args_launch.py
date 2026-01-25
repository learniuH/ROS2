from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

"""
需求: 在launch文件启动时动态地设置turtlesim_node的背景色
实现：
    1.声明参数（变量）
    2.调用参数（变量）
    3.执行launch文件时动态导入参数
终端调用: ros2 launch cpp01_launch py04_args_launch.py launch_arg_bgr:=255 launch_arg_bgg:=86 launch_arg_bgb:=255
"""

def generate_launch_description():
    # 1.声明参数（变量）
    launch_arg_bgr = DeclareLaunchArgument(name='launch_arg_bgr', default_value='255')
    launch_arg_bgg = DeclareLaunchArgument(name='launch_arg_bgg', default_value='255')
    launch_arg_bgb = DeclareLaunchArgument(name='launch_arg_bgb', default_value='255')

    # 2.调用参数（变量）
    turtle = Node(
        package='turtlesim',
        executable='turtlesim_node',
        parameters=[{'background_r': LaunchConfiguration('launch_arg_bgr'),
                     'background_g': LaunchConfiguration('launch_arg_bgg'),
                     'background_b': LaunchConfiguration('launch_arg_bgb')}]
    )

    return LaunchDescription([launch_arg_bgr, launch_arg_bgg, launch_arg_bgb, turtle])