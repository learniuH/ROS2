/*  
    需求：发布雷达坐标系中某个坐标点相对于雷达（laser）坐标系的位姿。
    步骤：
        1.包含头文件；
        2.初始化 ROS 客户端；
        3.定义节点类；
            3-1.创建坐标点发布方；
            3-2.创建定时器；
            3-3.组织并发布坐标点消息。
        4.调用 spin 函数，并传入对象指针；
        5.释放资源。

*/
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/point_stamped.hpp>

using namespace std::chrono_literals;

class PointTFBroadcaster: public rclcpp::Node
{
public:
    PointTFBroadcaster()
    : Node("point_tf_broadcaster")
    , x(0.0)
    {
        RCLCPP_INFO(this->get_logger(), "点位坐标变化发布对象创建！");

        // 3.1.创建发布方
        point_pub_ = this->create_publisher<geometry_msgs::msg::PointStamped>("point", 10);

        // 3.2.创建定时器
        timer_ = this->create_wall_timer(1s, std::bind(&PointTFBroadcaster::on_timer, this));
    }
private:
    rclcpp::Publisher<geometry_msgs::msg::PointStamped>::SharedPtr point_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
    double x;
    // 3.3.回调函数中组织并发布消息
    void on_timer()
    {
        // 1.组织消息
        geometry_msgs::msg::PointStamped point_stamped;
        point_stamped.header.stamp = this->now();
        point_stamped.header.frame_id = "laser";

        x += 0.05;
        point_stamped.point.x = x;
        point_stamped.point.y = 0.0;
        point_stamped.point.z = 0.0;

        // 2.发布消息
        point_pub_->publish(point_stamped);
    }
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<PointTFBroadcaster>());

    rclcpp::shutdown();

    return 0;
}