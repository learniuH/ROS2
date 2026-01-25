from launch import LaunchDescription
from launch_ros.actions import Node
from launch.event_handlers import OnProcessStart, OnProcessExit
from launch.actions import ExecuteProcess, RegisterEventHandler, LogInfo

"""
需求: 为turtlesim_node绑定事件: 
    - 节点启动时, 执行生成新的乌龟;
    - 节点关闭时, 执行日志输出
"""
def generate_launch_description():
    turtle = Node(package='turtlesim', executable='turtlesim_node')

    spawn = ExecuteProcess(
        cmd=["ros2 service call /spawn turtlesim/srv/Spawn \"{'x': 10, 'y': 10}\""],
        output='both',
        shell=True
    )

    # 注册事件
    event_start = RegisterEventHandler(
        # 创建handle
        OnProcessStart(
            target_action=turtle,
            on_start=spawn
        )
    )

    event_exit = RegisterEventHandler(
        OnProcessExit(
            target_action=turtle,
            on_exit=LogInfo(msg="turtlesim已退出！")
        )
    )

    return LaunchDescription([turtle, event_start, event_exit])