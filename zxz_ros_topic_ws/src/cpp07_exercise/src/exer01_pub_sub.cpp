/*
    需求：启动两个turtlesim_node节点，节点2中的乌龟自动掉头180度，用户可以通过键盘控制节点1中的乌龟运动，但是不能控制节点2中乌龟
         需要实现的功能：可以根据乌龟1的速度生成并发布控制乌龟2运动的速度指令，最终两个乌龟做镜像运动。

    订阅话题：/turtle1/pose
    消息类型：/turtlesim/msg/Pose
    消息接口：
        float32 x
        float32 y
        float32 theta

        float32 linear_velocity
        float32 angular_velocity
    
    发布话题：/diff/turtle1/cmd_vel
    消息类型：/geometry/msg/Twist
    消息接口：
        Vector3  linear
            float64 x
            float64 y
            float64 z
        Vector3  angular
            float64 x
            float64 y
            float64 z
    流程：
        1.创建发布方
        2.创建订阅方（订阅乌龟1位姿，解析速度）
        3.订阅方的回调函数处理速度，生成并发布控制乌龟2的速度指令
    BUG描述：
        乌龟1后退时，乌龟2仍然前进
    BUG原因：
        1.和乌龟姿态发布有关，当乌龟实际的速度为负数时，位姿中的速度仍是正数
        2.发布的乌龟2的速度，与位姿中的速度一致
        
        # turtlesim.cpp
        线速度 = 开方(x方向速度的平方 + y方向速度的平方)
        p->linear_velocity = std::sqrt(lin_vel_x * lin_vel_x + lin_vel_y * lin_vel_y);
    BUG修复：
        p->linear_velocity = lin_vel_x;
*/

#include <rclcpp/rclcpp.hpp>
#include <turtlesim/msg/pose.hpp>
#include <geometry_msgs/msg/twist.hpp>

class Pose2CmdVel: public rclcpp::Node
{
public:
    Pose2CmdVel()
    : Node("pose_to_cmd_vel")
    {
        RCLCPP_INFO(this->get_logger(), "案例1对象创建！");
        // 1.创建发布方
        cmd_vel_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/diff/turtle1/cmd_vel", 10);
        // 2.创建订阅方（订阅乌龟1位姿，解析速度）
        pose_sub_ = this->create_subscription<turtlesim::msg::Pose>("/turtle1/pose", 10, std::bind(&Pose2CmdVel::pose_cb, this, std::placeholders::_1));
    }
private:
    void pose_cb(const turtlesim::msg::Pose & pose)
    {
        // 3.订阅方的回调函数: 处理速度，生成并发布控制乌龟2的速度指令
        geometry_msgs::msg::Twist twist;
        twist.linear.x = pose.linear_velocity;
        twist.angular.z = -pose.angular_velocity;
        cmd_vel_pub_->publish(twist);
    }
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<Pose2CmdVel>());

    rclcpp::shutdown();
    return 0;
}