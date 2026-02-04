from launch import LaunchDescription
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
from launch.substitutions import Command, LaunchConfiguration
from ament_index_python.packages import get_package_share_directory
from launch.actions import DeclareLaunchArgument
import os

def generate_launch_description():
    """
        需求：加载urdf文件并在rviz2中显示机器人模型。
        核心：
            1.启动robot_state_publisher节点，该节点要以参数的方式加载urdf文件内容；
            2.启动rviz2节点
        优化：
            1.添加 joint_state_publisher 节点（当机器人有非固定关节时，必须包含该节点）；
            2.设置 rviz2 的默认配置文件；
            3.动态传入urdf文件，把urdf文件封装为参数；
    """
    cpp07_pkg_dir = get_package_share_directory('cpp07_urdf')
    
    default_model_path = os.path.join(
        cpp07_pkg_dir,
        'urdf/urdf', 
        'demo02_link.urdf'
    )
    default_rviz_path = os.path.join(
        cpp07_pkg_dir,
        'rviz',
        'demo.rviz'
    )

    model = DeclareLaunchArgument('model', default_value=default_model_path)
    # 1.启动robot_state_publisher节点，该节点要以参数的方式加载urdf文件内容；
    # 调用格式（注意标点符号）：
    # ros2 launch cpp07_urdf demo01_launch.py model:=`ros2 pkg prefix --share cpp07_urdf`/urdf/urdf/demo01_helloworld.urdf
    # ros2 pkg prefix --share cpp07_urdf 是一个指令，效果等价于 get_package_share_directory('cpp07_urdf')
    robot_description = ParameterValue(Command(['xacro ', LaunchConfiguration('model')]))
    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'robot_description': robot_description}]
    )

    # 2.启动 joint_state_publisher 节点发布非固定关节状态
    joint_state_publisher = Node(
        package='joint_state_publisher',
        executable='joint_state_publisher'
    )

    # 3.启动rviz2节点
    rviz = Node(
        package='rviz2',
        executable='rviz2',
        arguments=['-d', default_rviz_path]
    )

    return LaunchDescription([model, joint_state_publisher, robot_state_publisher, rviz])