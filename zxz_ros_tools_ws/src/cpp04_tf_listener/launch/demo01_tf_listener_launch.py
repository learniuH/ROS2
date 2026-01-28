from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    camera_to_base_tf = Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        arguments=[
            '--frame-id', 'base_link',
            '--child-frame-id', 'camera',
            '--x', '0.4',
            '--y', '0.0',
            '--z', '0.3'
        ]
    )

    laser_to_base_tf = Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        arguments=[
            '--frame-id', 'base_link',
            '--child-frame-id', 'laser',
            '--x', '0.2',
            '--y', '0.0',
            '--z', '0.4'
        ]
    )

    tf_listener = Node(
        package='cpp04_tf_listener',
        executable='demo01_tf_listener',
    )

    return LaunchDescription([camera_to_base_tf, laser_to_base_tf, tf_listener])