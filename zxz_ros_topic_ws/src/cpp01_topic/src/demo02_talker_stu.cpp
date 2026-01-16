/*
需求：以某个固定频率发布学生信息。
步骤：
    1.包含头文件；
    2.初始化 ROS2 客户端；
    3.定义节点类：
        3-1.创建发布方；
        3-2.创建定时器；
        3-3.组织消息并发布；
    4.调用spin函数，并传入节点对象指针；
    5.释放资源

*/

#include <rclcpp/rclcpp.hpp>
#include <base_interfaces_demo/msg/student.hpp>

class TalkerStu: public rclcpp::Node
{
public:
    TalkerStu()
    : Node("talker_stu")
    {

    }
};

int main(int argc, char * argv[])
{
    // 2.初始化 ROS2 客户端
    rclcpp::init(argc, argv);

    // 4.调用spin函数，并传入节点对象指针
    rclcpp::spin(std::make_shared<TalkerStu>());

    // 5.释放资源
    rclcpp::shutdown();
    return 0;
}