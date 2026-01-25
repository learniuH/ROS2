from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

"""
    演示Node类的使用：

    构造函数说明：
        :param: package the package in which the node executable can be found
        :param: executable the name of the executable to find if a package
            is provided or otherwise a path to the executable to run.
        :param: name the name of the node
        :param: namespace the ROS namespace for this Node
        :param: exec_name the label used to represent the process.
            Defaults to the basename of node executable.
        :param: parameters list of names of yaml files with parameter rules,
            or dictionaries of parameters.
        :param: remappings ordered list of 'to' and 'from' string pairs to be
            passed to the node as ROS remapping rules
        :param: ros_arguments list of ROS arguments for the node
        :param: arguments list of extra arguments for the node
"""

def generate_launch_description():
    arg1 = 'this is a argument to turtlesim_node'

    turtle = Node(
        package='turtlesim',
        executable='turtlesim_node',
        name='turtle_py',                                               # 节点名称重命名/turtle_py
        namespace='ns_py',                                              # 添加/ns_py命名空间
        exec_name='custom_turtle',                                      # 终端[custom_turtle]标签
        # parameters=[{'background_r': 255}],                           # 直接修改节点参数
        parameters=[os.path.join(get_package_share_directory('cpp01_launch'), 
                                 'config', 'ns_py__turtle_py.yaml')],   # 通过yaml文件设置节点参数（更常用）
        remappings=[('/ns_py/turtle1/cmd_vel', '/cmd_vel')],            # 话题重命名/turtle1/cmd_vel
        ros_arguments=['--remap', '/ns_py/turtle1/pose:=/pose'],        # 添加前缀 --ros-args --remap /ns_py/turtle/pose:=/pose 实现话题重命名
        arguments=[str(arg1)],                                          # turtlesim_node节点启动时传参
        respawn=True                                                    # 节点重启
    )

    return LaunchDescription([turtle])