/*
需求:编写动作客户端,可以发送一个整型数据到服务端,并处理服务端的连续反馈、最终响应结果
流程:
    前提:可以解析终端动态传入的参数
    3.自定义节点类
        3-1.创建动作客户端
        3-2.发送请求
        3-3.处理关于目标值的服务端响应(回调函数)
        3-4.处理连续反馈(回调函数)
        3-5.处理最终响应结果(回调函数)
*/

#include <rclcpp/rclcpp.hpp>
#include <base_interfaces_demo/action/progress.hpp>

class ProgressActionClient: public rclcpp::Node
{
public:
    ProgressActionClient()
    : Node("progress_action_client")
    {
        RCLCPP_INFO(this->get_logger(), "action 客户端创建！");
    }
};

int main(int argc, char * argv[])
{
    // 2.初始化 ROS2 节点
    rclcpp::init(argc, argv);

    // 4.调用spin函数
    rclcpp::spin(std::make_shared<ProgressActionClient>());

    // 5.释放资源
    rclcpp::shutdown();

    return 0;
}