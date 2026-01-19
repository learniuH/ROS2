/*
需求：创建参数客户端，查询或修改服务端参数
自定义节点类：
    1.创建参数客户端对象
    2.连接服务端
    3.参数查询
    4.修改参数
*/

#include <rclcpp/rclcpp.hpp>

class ParamClient: public rclcpp::Node
{
public:
    ParamClient()
    : Node("param_client")
    {
        RCLCPP_INFO(this->get_logger(), "参数客户端创建！");
    }
};

int main(int argc, char * argv[])
{
    // 2.初始化ROS2节点
    rclcpp::init(argc, argv);

    // 4.调用spin函数
    rclcpp::spin(std::make_shared<ParamClient>());

    // 5.释放资源
    rclcpp::shutdown();
    return 0;
}