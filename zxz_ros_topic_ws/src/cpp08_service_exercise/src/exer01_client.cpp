
#include <rclcpp/rclcpp.hpp>

class DistanceClient: public rclcpp::Node
{
public:
    DistanceClient()
    : Node("distance_client")
    {
        RCLCPP_INFO(this->get_logger(), "客户端创建！");
    }
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto client = std::make_shared<DistanceClient>();

    rclcpp::shutdown();

    return 0;
}