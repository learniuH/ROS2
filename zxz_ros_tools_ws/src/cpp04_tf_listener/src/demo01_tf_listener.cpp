
#include <rclcpp/rclcpp.hpp>

class StaticTFListener: public rclcpp::Node
{
public:
    StaticTFListener()
    : Node("static_tf_listener")
    {
        RCLCPP_INFO(this->get_logger(), "TF监听对象已创建！");
    }
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<StaticTFListener>());

    rclcpp::shutdown();

    return 0;
}