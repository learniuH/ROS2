

#include <rclcpp/rclcpp.hpp>

class DistanceServer: public rclcpp::Node
{
public:
    DistanceServer()
    : Node("distance_server")
    {

    }
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    
    rclcpp::spin(std::make_shared<DistanceServer>());

    rclcpp::shutdown();
    
    return 0;
}