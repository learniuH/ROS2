from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
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

    point_to_laser_tf = Node(
        package='cpp03_tf_broadcaster',
        executable='demo03_point_tf_broadcaster'
    )

    msg_filter_node = Node(
        package='cpp04_tf_listener',
        executable='demo02_msg_filter'
    )

    return LaunchDescription([laser_to_base_tf, point_to_laser_tf, msg_filter_node])