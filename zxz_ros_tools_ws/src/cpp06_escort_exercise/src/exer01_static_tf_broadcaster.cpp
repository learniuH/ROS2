
#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/static_transform_broadcaster.h>
#include <turtlesim/msg/pose.hpp>
#include <tf2/LinearMath/Quaternion.hpp>

class StaticTFBroadcaster: public rclcpp::Node
{
public:
    StaticTFBroadcaster()
    : Node("static_tf_broadcaster")
    {
        this->declare_parameter("name", "turtle1");
        this->declare_parameter("child_frame_id", "child_frame_id");
        this->declare_parameter("x", 1.0);
        this->declare_parameter("y", 1.0);

        turtle_name = this->get_parameter("name").as_string();
        child_frame_id = this->get_parameter("child_frame_id").as_string();
        x = this->get_parameter("x").as_double();
        y = this->get_parameter("y").as_double();

        static_tf_broadcaster_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);
        std::string pose_topic_name = "/" + turtle_name + "/pose";
        pose_sub_ = this->create_subscription<turtlesim::msg::Pose>(pose_topic_name, 10, std::bind(&StaticTFBroadcaster::pose_cb, this, std::placeholders::_1));
    }
private:
    std::shared_ptr<tf2_ros::StaticTransformBroadcaster> static_tf_broadcaster_;
    std::string turtle_name, child_frame_id;
    double x, y;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub_;
    void pose_cb(const turtlesim::msg::Pose & /* unused */)
    {
        geometry_msgs::msg::TransformStamped transform;
        transform.header.stamp = this->now();
        transform.header.frame_id = turtle_name;
        transform.child_frame_id = child_frame_id;

        transform.transform.translation.x = x;
        transform.transform.translation.y = y;
        transform.transform.translation.z = 0.0;

        tf2::Quaternion qtn;
        qtn.setRPY(0.0, 0.0, 0.0);
        transform.transform.rotation.x = qtn.x();
        transform.transform.rotation.y = qtn.y();
        transform.transform.rotation.z = qtn.z();
        transform.transform.rotation.w = qtn.w();

        /*
            void sendTransform(
            const geometry_msgs::msg::TransformStamped &transform)
        */
        static_tf_broadcaster_->sendTransform(transform);
    }
};
int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<StaticTFBroadcaster>());

    rclcpp::shutdown();

    return 0;
}