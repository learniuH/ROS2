
#include <rclcpp/rclcpp.hpp>
#include <turtlesim/srv/spawn.hpp>

using namespace std::chrono_literals;

class SpawnClient: public rclcpp::Node
{
public:
    SpawnClient()
    : Node("spawn_client")
    {
        // 声明节点参数，服务请求时使用
        this->declare_parameter("x", 3.0);
        this->declare_parameter("y", 3.0);
        this->declare_parameter("theta", 0.0);
        this->declare_parameter("name", "turtle2");

        client_ = this->create_client<turtlesim::srv::Spawn>("/spawn");
    }
    bool connect_server()
    {
        while (!client_->wait_for_service(1s))
        {
            if (!rclcpp::ok())
            {
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "用户中止！");
                return false;
            }
            RCLCPP_INFO(this->get_logger(), "服务器连接中...");
        }
        return true;
    }
    rclcpp::Client<turtlesim::srv::Spawn>::FutureAndRequestId request()
    {
        // turtlesim::srv::Spawn::Request::SharedPtr request;
        auto request = std::make_shared<turtlesim::srv::Spawn::Request>();
        request->x = this->get_parameter("x").as_double();
        request->y = this->get_parameter("y").as_double();
        request->theta = this->get_parameter("theta").as_double();
        request->name= this->get_parameter("name").as_string();
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
        if (name.empty())
        {
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "乌龟重名，/spawn服务失败！");
        }
        else 
        {
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "名为\"%s\"的乌龟生成成功！", name.c_str());
        }
    }
    else
    {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "服务响应失败！");
    }

    rclcpp::shutdown();

    return 0;
}