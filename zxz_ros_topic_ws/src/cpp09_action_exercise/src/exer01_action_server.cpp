
#include <rclcpp/rclcpp.hpp>

class NavServer: public rclcpp::Node
{
public:
    NavServer()
    : Node("nav_server")
    {
        RCLCPP_INFO(this->get_logger(), "动作服务端创建！");
    }
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<NavServer>());

    rclcpp::shutdown();

    return 0;
}