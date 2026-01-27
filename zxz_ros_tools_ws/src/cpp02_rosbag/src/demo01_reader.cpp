
#include <rclcpp/rclcpp.hpp>
#include <rosbag2_cpp/reader.hpp>
#include <geometry_msgs/msg/twist.hpp>

class BagPlayer: public rclcpp::Node
{
public:
    BagPlayer()
    : Node("bag_player")
    {
        RCLCPP_INFO(this->get_logger(), "消息回放对象创建！");
        // 1.创建回放对象
        reader_ = std::make_unique<rosbag2_cpp::Reader>();
        // 2.设置被读取文件
        reader_->open("bag_cpp");
        // 3.读取数据
        while (reader_->has_next())
        {
            auto twist = reader_->read_next<geometry_msgs::msg::Twist>();
            RCLCPP_INFO(this->get_logger(), "线速度：%.2f，角速度：%.2f", twist.linear.x, twist.angular.z);
        }
        // 4.关闭文件
        reader_->close();
    }
private:
    std::unique_ptr<rosbag2_cpp::Reader> reader_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<BagPlayer>());

    rclcpp::shutdown();

    return 0;
}