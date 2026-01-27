/*
需求：录制控制乌龟运动的速度指令
流程：
    1.创建录制对象
    2.设置磁盘文件
    3.写数据（创建速度订阅方，回调函数中执行写出操作）
*/

#include <rclcpp/rclcpp.hpp>
#include <rosbag2_cpp/writer.hpp>
#include <geometry_msgs/msg/twist.hpp>

class BagRecorder: public rclcpp::Node
{
public:
    BagRecorder()
    : Node("bag_recorder")
    {
        RCLCPP_INFO(this->get_logger(), "消息录制对象创建！");

        // 1.创建录制对象
        writer_ = std::make_unique<rosbag2_cpp::Writer>();
        // 2.设置磁盘文件
        writer_->open("bag_cpp");   // 相对路径，工作空间的直接子集

        sub_ = this->create_subscription<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10, std::bind(&BagRecorder::record_cmd_msg, this, std::placeholders::_1));
    }
private:
    std::unique_ptr<rosbag2_cpp::Writer> writer_;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_;
    void record_cmd_msg(std::shared_ptr<rclcpp::SerializedMessage> message)
    {
        /* void write(
        std::shared_ptr<rclcpp::SerializedMessage> message, 
        const std::string &topic_name, 
        const std::string &type_name, 
        const rclcpp::Time &time) */
        writer_->write(message, "/turtle1/cmd_vel", "geometry_msgs/msg/Twist", this->now());
        RCLCPP_INFO(this->get_logger(), "数据写出...");
    }
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<BagRecorder>());

    rclcpp::shutdown();

    return 0;
}