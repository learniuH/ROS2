from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from ament_index_python import get_package_share_directory
import os

def generate_launch_description():
    # rviz2文件默认路径
    package_path = get_package_share_directory('cpp03_tf_broadcaster')
    default_rviz_path = os.path.join(package_path, 'launch', 'static_tf_view.rviz')

    # 声明aunch参数
    frame_id = DeclareLaunchArgument(name='frame-id', default_value='base_link')
    child_frame_id = DeclareLaunchArgument(name='child-frame-id', default_value='laser')
    x = DeclareLaunchArgument(name='x', default_value='1.0')
    y = DeclareLaunchArgument(name='y', default_value='1.0')
    z = DeclareLaunchArgument(name='z', default_value='1.0')
    roll = DeclareLaunchArgument(name='roll', default_value='0.0')
    pitch = DeclareLaunchArgument(name='pitch', default_value='0.0')
    yaw = DeclareLaunchArgument(name='yaw', default_value='0.0')

    # 启动静态坐标变换节点并传入launch参数
    static_tf = Node(
        package='cpp03_tf_broadcaster', 
        executable='demo01_static_tf_broadcaster',
        arguments=[LaunchConfiguration('frame-id'), 
                   LaunchConfiguration('child-frame-id'), 
                   LaunchConfiguration('x'), 
                   LaunchConfiguration('y'), 
                   LaunchConfiguration('z'), 
                   LaunchConfiguration('roll'), 
                   LaunchConfiguration('pitch'), 
                   LaunchConfiguration('yaw')]
    )

    # 启动rviz2并加载默认rviz文件
    rviz2 = Node(
        package='rviz2', 
        executable='rviz2', 
        arguments=['-d', default_rviz_path]
    )

    return LaunchDescription([frame_id, child_frame_id, x, y, z, roll, pitch, yaw, static_tf, rviz2])