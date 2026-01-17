/*
需求:编写客户端,发送两个整型变量作为请求数据,并处理响应结果
流程:
    1.前提:main函数中判断提交的参数是否正确
    2.初始化 ROS2 客户端
    3.自定义节点类:
        3-1.创建客户端
        3-2.连接服务器(如果客户端无法连接到服务端,则不能发送请求)
        3-3.发送请求
    4.创建对象指针
        4-1.调用需要连接服务的函数,根据连接结果做进一步处理
        4-2.连接服务后,调用请求发送函数
        4-3.再处理响应结果
    5.资源释放
*/

#include <rclcpp/rclcpp.hpp>

class AddIntsClient: public rclcpp::Node
{
public:
    AddIntsClient()
    : Node("add_ints_server")
    {
        RCLCPP_INFO(this->get_logger(), "客户端创建！");
    }
};

int main(int argc, char * argv[])
{
    // 2.初始化 ROS2 节点
    rclcpp::init(argc, argv);

    // 创建客户端对象
    auto client = std::make_shared<AddIntsClient>();

    // 5.释放资源
    rclcpp::shutdown();
    return 0;
}