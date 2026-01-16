/*
需求：以某个固定频率发布学生信息。
步骤：
    1.包含头文件；
    2.初始化 ROS2 客户端；
    3.定义节点类：
        3-1.创建发布方；
        3-2.创建定时器；
        3-3.组织消息并发布；
    4.调用spin函数，并传入节点对象指针；
    5.释放资源

*/

#include <rclcpp/rclcpp.hpp>
#include <base_interfaces_demo/msg/student.hpp>

using namespace std::chrono_literals;

class TalkerStu: public rclcpp::Node
{
public:
    TalkerStu()
    : Node("talker_stu")
    {
        // 3-1.创建发布方
        publisher_ = this->create_publisher<base_interfaces_demo::msg::Student>("chatter_stu", 10);
        // 3-2.创建定时器
        timer_ = this->create_wall_timer(1s, bind(&TalkerStu::on_timer, this));
    }
private:
    void on_timer()
    {
        // 3-3.组织并发布学生消息
        auto stu = base_interfaces_demo::msg::Student();
        stu.name = "LearniuH";
        stu.age = 24;
        stu.height = 1.72;

        publisher_->publish(stu);
        RCLCPP_INFO(this->get_logger(), "发布的消息：%s, %d, %.2f", stu.name.c_str(), stu.age, stu.height);
    }
    rclcpp::Publisher<base_interfaces_demo::msg::Student>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
    // 2.初始化 ROS2 客户端
    rclcpp::init(argc, argv);

    // 4.调用spin函数，并传入节点对象指针
    rclcpp::spin(std::make_shared<TalkerStu>());

    // 5.释放资源
    rclcpp::shutdown();
    return 0;
}