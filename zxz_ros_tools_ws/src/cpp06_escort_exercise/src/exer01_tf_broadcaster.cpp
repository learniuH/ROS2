
#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <turtlesim/msg/pose.hpp>
#include <geometry_msgs/msg/transform_stamped.h>
#include <tf2/LinearMath/Quaternion.hpp>

class TFBroadcaster: public rclcpp::Node
{
public:
    TFBroadcaster()
    : Node("tf_broadcaster")
    {
        this->declare_parameter("name", "turtle1");
        broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);

        // 明确需要订阅的话题名称
        turtle_name = this->get_parameter("name").as_string();
        std::string pose_topic_name = "/" + turtle_name + "/pose";
        pose_sub_ = this->create_subscription<turtlesim::msg::Pose>(pose_topic_name, 10, std::bind(&TFBroadcaster::pose_cb, this, std::placeholders::_1));
    }
private:
    std::shared_ptr<tf2_ros::TransformBroadcaster> broadcaster_;
    std::string turtle_name;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub_;
    // 发布乌龟当前位置相对world（原点）的坐标变换
    void pose_cb(const turtlesim::msg::Pose & pose)
    {
        geometry_msgs::msg::TransformStamped transform;
        transform.header.stamp = this->now();
        transform.header.frame_id = "world";
        transform.child_frame_id = turtle_name;

        transform.transform.translation.x = pose.x;
        transform.transform.translation.y = pose.y;
        transform.transform.translation.z = 0.0;

        tf2::Quaternion qtn;
        qtn.setRPY(0.0, 0.0, pose.theta);
        transform.transform.rotation.x = qtn.x();
        transform.transform.rotation.y = qtn.y();
        transform.transform.rotation.z = qtn.z();
        transform.transform.rotation.w = qtn.w();
        /*
            void sendTransform(
            const geometry_msgs::msg::TransformStamped &transform)
        */
        broadcaster_->sendTransform(transform);
    }
};
int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<TFBroadcaster>());

    rclcpp::shutdown();

    return 0;
}