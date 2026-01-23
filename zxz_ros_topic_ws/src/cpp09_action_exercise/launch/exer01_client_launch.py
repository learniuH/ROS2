from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess
import random

def generate_launch_description():
    x = random.uniform(2, 10)
    y = random.uniform(2, 10)
    theta = random.uniform(0, 6.28)

    spawn_turtle = ExecuteProcess(
        cmd=[f"ros2 service call /spawn turtlesim/srv/Spawn \"{{'x': {x}, 'y': {y}, 'theta': {theta}, 'name': 'turtle2'}}\""],
        output='both',
        shell=True
    )
    
    nav_client = Node(
        package='cpp09_action_exercise', 
        executable='exer01_action_client',
        arguments=[str(x), str(y), str(theta)]
    )

    return LaunchDescription([spawn_turtle, nav_client])