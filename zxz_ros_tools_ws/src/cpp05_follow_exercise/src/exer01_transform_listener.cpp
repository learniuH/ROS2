
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
        RCLCPP_INFO(this->get_logger(), "tf_listener对象创建！");
        // 3-1.声明参数服务
        this->declare_parameter("frame_id", "turtle2");
        this->declare_parameter("child_frame_id", "turtle1");
        frame_id = this->get_parameter("frame_id").as_string();
        child_frame_id = this->get_parameter("child_frame_id").as_string();

        // 3-2.创建缓存对象
        buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());

        // 3-3.创建监听器
        listener_ = std::make_shared<tf2_ros::TransformListener>(*buffer_);

        // 3-4.创建速度发布方
        std::string cmd_topic = "/" + frame_id + "/cmd_vel";
        cmd_pub_ = this->create_publisher<geometry_msgs::msg::Twist>(cmd_topic, 10);
        
        // 3-5.创建定时器，实现坐标的变换，并生成速度指令并发布
        timer_ = this->create_wall_timer(1s, std::bind(&TFListener::on_timer, this));
    }
private:
    std::string frame_id, child_frame_id;
    std::shared_ptr<tf2_ros::Buffer> buffer_;
    std::shared_ptr<tf2_ros::TransformListener> listener_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
    void on_timer()
    {
        try
        {
            // 1.实现坐标变换
            auto transform_stamped = buffer_->lookupTransform(frame_id, child_frame_id, tf2::TimePointZero);

            // linear = 系数 * sqrt(x ** 2 + y ** 2);
            // angular = 系数 * atan2(y / x); 反正切
            geometry_msgs::msg::Twist twist;
            twist.linear.x = 0.5 * sqrt(pow(transform_stamped.transform.translation.x, 2) + pow(transform_stamped.transform.translation.y, 2));
            twist.angular.z = 1.0 * atan2(transform_stamped.transform.translation.y, transform_stamped.transform.translation.x);
            cmd_pub_->publish(twist);
        }
        catch(const tf2::LookupException& e)
        {
            RCLCPP_INFO(this->get_logger(), "坐标变换异常，%s", e.what());
            return;
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