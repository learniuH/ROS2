/*
需求：启动 turtlesim_node 节点，编写程序，发布乌龟(turtle1)相对于窗体(world)的位姿
流程：
    1.创建动态广播器
    2.创建乌龟位姿订阅方
    3.回调函数中获取乌龟位姿，成相对关系，并发布
*/

#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_broadcaster.hpp>
#include <turtlesim/msg/pose.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <tf2/LinearMath/Quaternion.hpp>

class DynamicTFBroadcaster: public rclcpp::Node
{
public:
    DynamicTFBroadcaster()
    : Node("dynamic_tf_broadcaster")
    {
        RCLCPP_INFO(this->get_logger(), "动态坐标变换发布对象创建！");
        
        // 1.创建动态广播器
        broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);

        // 2.创建乌龟位姿订阅方
        pose_sub_ = this->create_subscription<turtlesim::msg::Pose>("/turtle1/pose", 10, std::bind(&DynamicTFBroadcaster::handle_pose_cb, this, std::placeholders::_1));
    }
private:
    std::shared_ptr<tf2_ros::TransformBroadcaster> broadcaster_;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub_;
    void handle_pose_cb(const turtlesim::msg::Pose & pose)
    {
        // 3.回调函数中获取乌龟位姿，成相对关系，并发布
        // 3-1.组织消息
        geometry_msgs::msg::TransformStamped transform;
        transform.header.stamp = this->now();
        transform.header.frame_id = "world";
        transform.child_frame_id = "turtle1";

        transform.transform.translation.x = pose.x;
        transform.transform.translation.y = pose.y;
        transform.transform.translation.z = 0.0;

        tf2::Quaternion qtn;
        qtn.setRPY(0.0, 0.0, pose.theta);
        transform.transform.rotation.x = qtn.x();
        transform.transform.rotation.y = qtn.y();
        transform.transform.rotation.z = qtn.z();
        transform.transform.rotation.w = qtn.w();

        // void sendTransform(const geometry_msgs::msg::TransformStamped &transform)
        broadcaster_->sendTransform(transform);
    }
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<DynamicTFBroadcaster>());

    rclcpp::shutdown();

    return 0;
}