from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess, RegisterEventHandler
from launch.event_handlers import OnProcessExit

def generate_launch_description():
    # 1.启动两个 turtlesim_node, 其中一个要设置 namespace
    turtle1 = Node(package='turtlesim', executable='turtlesim_node')
    turtle2 = Node(package='turtlesim', executable='turtlesim_node', namespace='diff')
    
    # 2.控制第二只乌龟掉头
    t_rotate_proc = ExecuteProcess(
        cmd=['ros2 action send_goal /diff/turtle1/rotate_absolute turtlesim/action/RotateAbsolute "{\'theta\': 3.14}"'],
        output='both',
        shell=True
    )

    # 3.调用自定义节点（必须在第二只乌龟掉头结束后）
    exer01 = Node(package='cpp07_topic_exercise', executable='exer01_pub_sub')
    # 通过注册事件完成控制节点的执行顺序
    # 创建事件注册对象，在对象中声明针对哪个目标节点，在哪个事件触发时，执行哪种操作
    run_exer01_event = RegisterEventHandler(
        # 创建一个新对象-触发动作（目标节点、触发执行的事件）
        event_handler=OnProcessExit(target_action=t_rotate_proc, on_exit=exer01)
    )

    # 4.通过新终端启动键盘控制节点
    keyboard_ctrl = Node(package='turtlesim', executable='turtle_teleop_key', prefix='gnome-terminal --', output='screen')

    return LaunchDescription([turtle1, turtle2, t_rotate_proc, run_exer01_event, keyboard_ctrl])