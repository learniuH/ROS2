from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

import random

def generate_launch_description():
    # 声明用于turtlesim的spawn服务的name参数
    launch_arg_spawn_left = DeclareLaunchArgument(name='spawn_left_name', default_value='spawn_left')
    launch_arg_spawn_right = DeclareLaunchArgument(name='spawn_right_name', default_value='spawn_right')
    launch_arg_spawn_back = DeclareLaunchArgument(name='spawn_back_name', default_value='spawn_back')

    # 声明turtlesim乌龟的三个护航点坐标系名称
    launch_arg_escort_left_name = DeclareLaunchArgument(name='escort_left_name', default_value='escort_left')
    launch_arg_escort_right_name = DeclareLaunchArgument(name='escort_right_name', default_value='escort_right')
    launch_arg_escort_back_name = DeclareLaunchArgument(name='escort_back_name', default_value='escort_back')

    # 声明turtlesim乌龟的三个护航点坐标，default必须是字符串
    launch_arg_escort_left_point = DeclareLaunchArgument(name='escort_left_point', default_value='1.0')
    launch_arg_escort_right_point = DeclareLaunchArgument(name='escort_right_point', default_value='-1.0')
    launch_arg_escort_back_point = DeclareLaunchArgument(name='escort_back_point', default_value='-1.0')

    # 启动turtlesim节点，并发布其相对world（原点）的坐标变换
    turtle_node = Node(package='turtlesim', executable='turtlesim_node')
    turtle_node_tf = Node(package='cpp06_escort_exercise', executable='exer01_tf_broadcaster', name='turtlesim_tf')
    # 发布turtlesim节点的三个护航点的静态坐标变换
    escort_left_static_tf = Node(
        package='cpp06_escort_exercise',
        executable='exer01_static_tf_broadcaster',
        name='escort_left_static_tf',
        parameters=[
            {'child_frame_id': LaunchConfiguration('escort_left_name')},
            {'x': 0.0},
            {'y': LaunchConfiguration('escort_left_point')},
        ]
    )
    escort_right_static_tf = Node(
        package='cpp06_escort_exercise',
        executable='exer01_static_tf_broadcaster',
        name='escort_right_static_tf',
        parameters=[
            {'child_frame_id': LaunchConfiguration('escort_right_name')},
            {'x': 0.0},
            {'y': LaunchConfiguration('escort_right_point')},
        ]
    )
    escort_back_static_tf = Node(
        package='cpp06_escort_exercise',
        executable='exer01_static_tf_broadcaster',
        name='escort_back_static_tf',
        parameters=[
            {'child_frame_id': LaunchConfiguration('escort_back_name')},
            {'x': LaunchConfiguration('escort_back_point')},
            {'y': 0.0},
        ]
    )

    # 生成护航乌龟
    spawn_left = Node(
        package='cpp06_escort_exercise', 
        executable='exer01_spawn',
        name='spawn_left',
        parameters=[
            {'x': random.uniform(1.0, 10.0)},
            {'y': random.uniform(1.0, 10.0)},
            {'theta': random.uniform(0.0, 6.28)},
            {'name': LaunchConfiguration('spawn_left_name')}
        ]
    )
    # 发布左乌龟相对world（原点）的坐标变换
    spawn_left_tf = Node(
        package='cpp06_escort_exercise',
        executable='exer01_tf_broadcaster',
        name='spawn_left_tf',
        parameters=[{'name': LaunchConfiguration('spawn_left_name')}]
    )
    # 计算左乌龟与tutlesim左护航点的坐标变换
    # 并控制左乌龟到左护航点
    spawn_left_ctl = Node(
        package='cpp06_escort_exercise',
        executable='exer01_tf_listener',
        name='spawn_left_ctrl',
        parameters=[
            {'target_frame': LaunchConfiguration('spawn_left_name')},
            {'source_frame': LaunchConfiguration('escort_left_name')}
        ]
    )

    # 生成护航乌龟
    spawn_right = Node(
        package='cpp06_escort_exercise', 
        executable='exer01_spawn',
        name='spawn_right',
        parameters=[
            {'x': random.uniform(1.0, 10.0)},
            {'y': random.uniform(1.0, 10.0)},
            {'theta': random.uniform(0.0, 6.28)},
            {'name': LaunchConfiguration('spawn_right_name')}
        ]
    )
    # 发布右乌龟相对world（原点）的坐标变换
    spawn_right_tf = Node(
        package='cpp06_escort_exercise',
        executable='exer01_tf_broadcaster',
        name='spawn_right_tf',
        parameters=[{'name': LaunchConfiguration('spawn_right_name')}]
    )
    # 计算右乌龟与tutlesim右护航点的坐标变换
    # 并控制右乌龟到右护航点
    spawn_right_ctl = Node(
        package='cpp06_escort_exercise',
        executable='exer01_tf_listener',
        name='spawn_right_ctrl',
        parameters=[
            {'target_frame': LaunchConfiguration('spawn_right_name')},
            {'source_frame': LaunchConfiguration('escort_right_name')}
        ]
    )
    
    # 生成护航乌龟
    spawn_back = Node(
        package='cpp06_escort_exercise', 
        executable='exer01_spawn',
        name='spawn_back',
        parameters=[
            {'x': random.uniform(1.0, 10.0)},
            {'y': random.uniform(1.0, 10.0)},
            {'theta': random.uniform(0.0, 6.28)},
            {'name': LaunchConfiguration('spawn_back_name')}
        ]
    )
    # 发布后乌龟相对world（原点）的坐标变换
    spawn_back_tf = Node(
        package='cpp06_escort_exercise',
        executable='exer01_tf_broadcaster',
        name='spawn_back_tf',
        parameters=[{'name': LaunchConfiguration('spawn_back_name')}]
    )
    # 计算后乌龟与tutlesim后护航点的坐标变换
    # 并控制后乌龟到后护航点
    spawn_back_ctl = Node(
        package='cpp06_escort_exercise',
        executable='exer01_tf_listener',
        name='spawn_back_ctrl',
        parameters=[
            {'target_frame': LaunchConfiguration('spawn_back_name')},
            {'source_frame': LaunchConfiguration('escort_back_name')}
        ]
    )

    keyboard = Node(
        package='teleop_twist_keyboard',
        executable='teleop_twist_keyboard',
        ros_arguments=['--remap', '/cmd_vel:=/turtle1/cmd_vel'],
        # arguments=['--ros-args', '--remap', '/cmd_vel:=/turtle1/cmd_vel'],
        prefix='gnome-terminal --',
        output='screen'
    )

    return LaunchDescription([launch_arg_spawn_left, launch_arg_spawn_right, launch_arg_spawn_back, 
                              launch_arg_escort_left_name, launch_arg_escort_right_name, launch_arg_escort_back_name,
                              launch_arg_escort_left_point, launch_arg_escort_right_point, launch_arg_escort_back_point,
                              turtle_node, turtle_node_tf, escort_left_static_tf, escort_right_static_tf, escort_back_static_tf,
                              spawn_left, spawn_left_tf, spawn_left_ctl,
                              spawn_right, spawn_right_tf, spawn_right_ctl,
                              spawn_back, spawn_back_tf, spawn_back_ctl, keyboard])