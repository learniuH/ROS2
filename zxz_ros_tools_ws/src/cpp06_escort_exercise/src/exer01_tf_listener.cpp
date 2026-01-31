
#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/msg/twist.hpp>

using namespace std::chrono_literals;

class TFListener: public rclcpp::Node
{
public:
    TFListener()
    : Node("tf_listener")
    {
        this->declare_parameter("target_frame", "turtle2");
        this->declare_parameter("source_frame", "turtle1");
        target_frame = this->get_parameter("target_frame").as_string();
        source_frame = this->get_parameter("source_frame").as_string();

        buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());

        listener_ = std::make_shared<tf2_ros::TransformListener>(*buffer_, this);

        timer_ = this->create_wall_timer(1s, std::bind(&TFListener::on_timer, this));

        std::string cmd_topic_name = "/" + target_frame + "/cmd_vel";
        cmd_pub_ = this->create_publisher<geometry_msgs::msg::Twist>(cmd_topic_name, 10);
    }
private:
    std::unique_ptr<tf2_ros::Buffer> buffer_;
    std::string target_frame, source_frame;
    std::shared_ptr<tf2_ros::TransformListener> listener_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub_;
    void on_timer()
    {
        try
        {
            geometry_msgs::msg::TransformStamped transform_stamped;
            geometry_msgs::msg::Twist twist;
            /*
                geometry_msgs::msg::TransformStamped lookupTransform(
                const std::string &target_frame, 
                const std::string &source_frame, 
                const tf2::TimePoint &time) const
            */
            transform_stamped = buffer_->lookupTransform(target_frame, source_frame, tf2::TimePointZero);
            double x = transform_stamped.transform.translation.x;
            double y = transform_stamped.transform.translation.y;

            twist.linear.x = 0.5 * sqrt(pow(x, 2) + pow(y, 2));
            twist.angular.z = 1.0 * atan2(y, x);
            cmd_pub_->publish(twist);
        }
        catch(const tf2::LookupException& e)
        {
            RCLCPP_INFO(this->get_logger(), "%s", e.what());
        }
        
    }
};
int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<TFListener>());

    rclcpp::shutdown();
    
    return 0;
}