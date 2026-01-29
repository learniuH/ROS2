
#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/buffer.hpp>
#include <tf2_ros/transform_listener.hpp>
#include <tf2_ros/create_timer_ros.hpp>
#include <tf2_ros/message_filter.hpp>
#include <message_filters/subscriber.hpp>
#include <geometry_msgs/msg/point_stamped.hpp>

#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>

using namespace std::chrono_literals;

class TFPointListener: public rclcpp::Node
{
public:
    TFPointListener()
    : Node("tf_point_listener")
    {
        RCLCPP_INFO(this->get_logger(), "坐标点转换对象创建！");
        buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
        /* 
            CreateTimerROS(
            rclcpp::node_interfaces::NodeBaseInterface::SharedPtr node_base, 
            rclcpp::node_interfaces::NodeTimersInterface::SharedPtr node_timers, 
            rclcpp::CallbackGroup::SharedPtr callback_group = nullptr) 
        */
        timer_ = std::make_shared<tf2_ros::CreateTimerROS>(
            this->get_node_base_interface(),
            this->get_node_timers_interface()
        );
        /* void setCreateTimerInterface(tf2_ros::CreateTimerInterface::SharedPtr create_timer_interface) */
        buffer_->setCreateTimerInterface(timer_);

        listener_ = std::make_shared<tf2_ros::TransformListener>(*buffer_);

        point_sub_.subscribe(this, "point");

        /* 
            MessageFilter<F, TimeRepT, TimeT>(
            F &f, 
            tf2_ros::Buffer &buffer, 
            const std::string &target_frame, 
            uint32_t queue_size, 
            const rclcpp::node_interfaces::NodeLoggingInterface::SharedPtr &node_logging, 
            const rclcpp::node_interfaces::NodeClockInterface::SharedPtr &node_clock, 
            std::chrono::duration<TimeRepT, TimeT> buffer_timeout) 
        */
        filter_ = std::make_shared<tf2_ros::MessageFilter<geometry_msgs::msg::PointStamped>>(
            point_sub_,
            *buffer_,
            "base_link",
            10,
            this->get_node_logging_interface(),
            this->get_node_clock_interface(),
            1s
        );
        filter_->registerCallback(&TFPointListener::transform_point, this);
    }
private:
    std::unique_ptr<tf2_ros::Buffer> buffer_;
    std::shared_ptr<tf2_ros::TransformListener> listener_;
    std::shared_ptr<tf2_ros::CreateTimerROS> timer_;
    std::shared_ptr<tf2_ros::MessageFilter<geometry_msgs::msg::PointStamped>> filter_;
    /**
     * \brief Empty constructor, use subscribe() to subscribe to a topic
        
        Subscriber() = default;
     */
    message_filters::Subscriber<geometry_msgs::msg::PointStamped> point_sub_;
    void transform_point(const geometry_msgs::msg::PointStamped & point)
    {
        // 实现坐标变换，必须包含头文件 tf2_geometry_msgs/tf2_geometry_msgs.hpp
        /* T transform<T>(const T &in, const std::string &target_frame, tf2::Duration timeout = tf2::durationFromSec((0.0))) */
        auto out = buffer_->transform(point, "base_link");
        RCLCPP_INFO(this->get_logger(), "point相对base_link的坐标：(%.2f, %.2f, %.2f)", out.point.x, out.point.y, out.point.z);
    }
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<TFPointListener>());

    rclcpp::shutdown();

    return 0;
}