from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess
from launch.substitutions import FindExecutable

def generate_launch_description():
    turtle = Node(package='turtlesim', executable='turtlesim_node')

    topic_info = ExecuteProcess(
        cmd=['ros2 topic echo /turtle1/pose'],
        # cmd=[FindExecutable(name="ros2"), "topic", "echo", "/turtle1/pose"],
        output='both',      # 同时输出到终端和log文件
        shell=True          # 使用shell执行cmd命令
    )

    return LaunchDescription([turtle, topic_info])