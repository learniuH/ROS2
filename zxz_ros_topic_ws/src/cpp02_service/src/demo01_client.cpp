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
#include <base_interfaces_demo/srv/add_ints.hpp>

using base_interfaces_demo::srv::AddInts;
using namespace std::chrono_literals;

class AddIntsClient: public rclcpp::Node
{
public:
    AddIntsClient()
    : Node("add_ints_client")
    {
        // 3-1.创建客户端
        client_ = this->create_client<AddInts>("add_ints");
        /*
        create_client
        模板:服务接口
        参数:服务话题名称
        返回值:服务对象指针
        */
        RCLCPP_INFO(this->get_logger(), "客户端创建！");
    }
    // 3-2.连接服务器（如果客户端无法连接到服务器，则不能发送请求），返回值为是否连接成功
    bool connect_server()
    {
        // client_->wait_for_service(1s); // 在指定超时时间内连接服务器，若连接成功则返回 true，否则返回 false
        while (!client_->wait_for_service(1s))
        {
            // 对键盘终止做出处理
            if (!rclcpp::ok())  // 1.判断 Ctrl + C 按下
            {
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "用户终止客户端！");
                return false;
            }
            RCLCPP_INFO(this->get_logger(), "服务器连接中...");
        }
        return true;
    }
    // 3-3.发送请求
    // 编写发送请求函数 --- 参数为两个整型数据,返回值为服务端的返回结果
    rclcpp::Client<AddInts>::FutureAndRequestId send_request(int num1, int num2)
    {
        // 组织请求数据
        auto request = std::make_shared<AddInts::Request>();
        request->num1 = num1;
        request->num2 = num2;

        // rclcpp::Client<base_interfaces_demo::srv::AddInts>::FutureAndRequestId 
        // async_send_request(std::shared_ptr<base_interfaces_demo::srv::AddInts_Request> request)
        return client_->async_send_request(request);
    }
private:
    rclcpp::Client<AddInts>::SharedPtr client_;
};

int main(int argc, char * argv[])
{
    if (argc != 3)
    {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "请提交两个整性数据！");
        return 1;
    }

    // 2.初始化 ROS2 节点
    rclcpp::init(argc, argv);

    // 创建客户端对象
    auto client = std::make_shared<AddIntsClient>();

    // 调用客户端对象的连接服务器功能
    if (!client->connect_server())
    {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "服务器连接失败，程序退出！");
        /*
        rclcpp::get_logger("name") 创建 logger 对象不依赖于 context
        */
       return 0;
    }

    auto future = client->send_request(atoi(argv[1]), atoi(argv[2]));
    if (rclcpp::spin_until_future_complete(client, future) == rclcpp::FutureReturnCode::SUCCESS)
    {
        RCLCPP_INFO(client->get_logger(), "sum = %d", future.get()->sum);
    }
    else 
    {
        RCLCPP_INFO(client->get_logger(), "响应失败！");
    }

    // 5.释放资源
    rclcpp::shutdown();
    return 0;
}