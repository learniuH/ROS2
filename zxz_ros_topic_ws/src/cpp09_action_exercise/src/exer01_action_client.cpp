

#include <rclcpp/rclcpp.hpp>

class NavClient: public rclcpp::Node
{
public:
    NavClient()
    : Node("nav_client")
    {
        RCLCPP_INFO(this->get_logger(), "动作客户端创建！");
    }
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<NavClient>());

    rclcpp::shutdown();

    return 0;
}