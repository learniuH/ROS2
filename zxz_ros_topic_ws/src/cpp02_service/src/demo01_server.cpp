/*
需求:编写服务端,接收客户端发送的请求,提取其中两个整型数据,相加后将结果响应回客户端.
步骤:
    1.包含头文件;
    2.初始化 ROS2 节点
    3.定义节点类
        3-1.创建服务端
        3-2.回调函数:解析请求数据,并响应结果
    4.调用spin函数,并传入节点对象指针
    5.释放资源
*/

// 测试方法：
// ros2 service call /add_ints base_interfaces_demo/srv/AddInts "{'num1': 10, 'num2': 30}"

#include <rclcpp/rclcpp.hpp>
#include <base_interfaces_demo/srv/add_ints.hpp>

using base_interfaces_demo::srv::AddInts;

class AddIntsServer: public rclcpp::Node
{
public:
    AddIntsServer()
    : Node("add_ints_server")
    {
        RCLCPP_INFO(this->get_logger(), "服务端创建！");
        // 3-1.创建服务端
        server_ = this->create_service<AddInts>("add_ints", std::bind(&AddIntsServer::add, this, std::placeholders::_1, std::placeholders::_2));
    }
private:
    void add(const AddInts::Request::SharedPtr req, const AddInts::Response::SharedPtr res)
    {
        // 3-2.回调函数，解析请求数据，并响应结果
        res->sum = req->num1 + req->num2;
        RCLCPP_INFO(this->get_logger(), "%d + %d = %d", req->num1, req->num2, res->sum);
    }
    rclcpp::Service<AddInts>::SharedPtr server_;
};

int main(int argc, char * argv[])
{
    // 2.初始化 ROS2 节点
    rclcpp::init(argc, argv);

    // 4.调用spin函数,并传入节点对象指针
    rclcpp::spin(std::make_shared<AddIntsServer>());

    // 5.释放资源
    rclcpp::shutdown();
    return 0;
}