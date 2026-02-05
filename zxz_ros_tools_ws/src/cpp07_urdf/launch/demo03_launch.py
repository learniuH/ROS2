from launch import LaunchDescription
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
from launch.substitutions import Command, LaunchConfiguration
from launch.actions import DeclareLaunchArgument
from ament_index_python.packages import get_package_share_directory
import os

"""
    问题描述：通过 joint_state_publisher_gui 让关节运动到指定位置之后，关节存在“抖动”（在初始位置和指定位置之间抖动）
    解   决：不再启动 joint_state_publisher 节点
    原   因：
            1.joint_state_publisher与joint_state_publisher_gui作用一致，都会发布非固定关节的运动信息。
            2.robot_state_publisher会订阅关节的运动信息，并生成坐标变换数据广播。
            3.joint_state_publisher或joint_state_publisher_gui有一个存在时，就会发布关节运动信息，进而就能生成坐标变换
                当两者都不启动时，坐标树生成不了，机器人模型显示异常。
                当两者都存在时，joint_state_publisher一直发布初始关节位姿信息，joint_state_publisher_gui发布指定的关节位姿信息，
                最终，订阅两者发布的消息，产生了抖动的效果。
"""

def generate_launch_description():
    package_path = get_package_share_directory('cpp07_urdf')
    
    default_urdf_path = os.path.join(
        package_path,
        'urdf/urdf',
        'demo03_joint.urdf'
    )
    default_rviz_path = os.path.join(
        package_path,
        'rviz',
        'demo.rviz'
    )

    model = DeclareLaunchArgument(name='model', default_value=default_urdf_path)

    robot_description = ParameterValue(Command(['xacro ', LaunchConfiguration('model')]))

    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'robot_description': robot_description}]
    )

    joint_state_publisher_gui = Node(
        package='joint_state_publisher_gui',
        executable='joint_state_publisher_gui'
    )

    rviz = Node(
        package='rviz2',
        executable='rviz2',
        arguments=['-d', default_rviz_path]
    )

    return LaunchDescription([model, robot_state_publisher, joint_state_publisher_gui, rviz])
