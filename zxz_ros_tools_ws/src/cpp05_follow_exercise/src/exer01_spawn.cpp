/*  
  需求：编写客户端，发送请求生成一只新的乌龟。
  步骤：
    1.包含头文件；
    2.初始化 ROS2 客户端；
    3.定义节点类；
      3-1.声明并获取参数；
      3-2.创建客户端；
      3-3.等待服务连接；
      3-4.组织请求数据并发送；
    4.创建对象指针调用其功能,并处理响应；
    5.释放资源。

*/

#include <rclcpp/rclcpp.hpp>
#include <turtlesim/srv/spawn.hpp>

using namespace std::chrono_literals;

class SpawnClient: public rclcpp::Node
{
public:
    SpawnClient()
    : Node("spawn_client")
    {
        RCLCPP_INFO(this->get_logger(), "spawn客户端已创建！");

        // 3-1.声明参数
        this->declare_parameter("x", 3.0);
        this->declare_parameter("y", 3.0);
        this->declare_parameter("theta", 0.0);
        this->declare_parameter("name", "turtle_spawn");

        // 3-2.创建客户端；
        client_ = this->create_client<turtlesim::srv::Spawn>("/spawn");
    }
    // 3-3.等待服务连接；
    bool connect_server()
    {
        while (!client_->wait_for_service(1s))
        {
            if (!rclcpp::ok())
            {
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Keyboard Interupted!");
                return false;
            }
            RCLCPP_INFO(this->get_logger(), "connecting...");
        }
        return true;
    }
    // 3-4.组织请求数据并发送；
    rclcpp::Client<turtlesim::srv::Spawn>::FutureAndRequestId request()
    {
        auto request = std::make_shared<turtlesim::srv::Spawn::Request>();
        request->x = this->get_parameter("x").as_double();
        request->y = this->get_parameter("y").as_double();
        request->theta = this->get_parameter("theta").as_double();
        request->name = this->get_parameter("name").as_string();
        /* 
            rclcpp::Client<turtlesim::srv::Spawn>::FutureAndRequestId async_send_request(
            std::shared_ptr<turtlesim::srv::Spawn_Request> request) 
        */
        return client_->async_send_request(request);
    }
private:
    rclcpp::Client<turtlesim::srv::Spawn>::SharedPtr client_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto client = std::make_shared<SpawnClient>();
    if (!client->connect_server())
    {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "服务连接失败！");
        return 1;
    }
    auto future = client->request();
    if (rclcpp::spin_until_future_complete(client, future) == rclcpp::FutureReturnCode::SUCCESS)
    {
        std::string name = future.get()->name;
        // 如果生成新乌龟时，新乌龟重名了，那么也会响应成功，但是其实
        // 没有生成新乌龟，且响应结果中，乌龟的名称是空字符串（正常应该是请求的乌龟的名称）
        if (name.empty())
        {
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "乌龟重名生成失败！");
        }
        else
        {
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "乌龟%s生成成功！", name.c_str());
        }
    }
    else
    {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "请求异常！");
    }

    rclcpp::shutdown();

    return 0;
}