/*   
  需求：发布乌龟坐标系到窗口坐标系的坐标变换。
  步骤：
    1.包含头文件；
    2.初始化 ROS 客户端；
    3.定义节点类；
      3-1.声明并解析乌龟名称参数；
      3-2.创建动态坐标变换发布方；
      3-3.创建乌龟位姿订阅方；
      3-4.根据订阅到的乌龟位姿生成坐标帧并广播。
    4.调用 spin 函数，并传入对象指针；
    5.释放资源。

*/

#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <turtlesim/msg/pose.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <tf2/LinearMath/Quaternion.hpp>

class TFBroadcaster: public rclcpp::Node
{
public:
    TFBroadcaster()
    : Node("tf_broadcaster")
    {
        RCLCPP_INFO(this->get_logger(), "tf广播对象创建！");

        // 3-1.声明并解析乌龟名称参数；
        this->declare_parameter("turtle_name", "turtle1");
        turtle_name = this->get_parameter("turtle_name").as_string();

        // 3-2.创建动态坐标变换发布方；
        tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);

        // 3-3.创建乌龟位姿订阅方；
        std::string topic_name = "/" + turtle_name + "/pose";
        pose_sub_ = this->create_subscription<turtlesim::msg::Pose>(topic_name, 10, std::bind(&TFBroadcaster::pose_cb, this, std::placeholders::_1));
    }
private:
    std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub_;
    std::string turtle_name;
    // 3-4.根据订阅到的乌龟位姿生成坐标帧并广播。  
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
        
        // void sendTransform(const geometry_msgs::msg::TransformStamped &transform)
        tf_broadcaster_->sendTransform(transform);
    }
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<TFBroadcaster>());

    rclcpp::shutdown();

    return 0;
}