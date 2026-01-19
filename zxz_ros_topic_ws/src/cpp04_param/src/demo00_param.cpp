/*
自定义节点对象
    1.参数对象创建
    2.参数对象解析（获取键、值、将值转换成字符串...）

*/

#include <rclcpp/rclcpp.hpp>

class MyParam: public rclcpp::Node
{
public:
    MyParam()
    : Node("my_param")
    {
        RCLCPP_INFO(this->get_logger(), "参数API使用");

        // 1.参数对象创建
        rclcpp::Parameter p1("robot_name", "Lobot");
        rclcpp::Parameter p2("robot_height", 1.79);
        rclcpp::Parameter p3("robot_wheel_nums", 4);

        // 2.参数对象解析（获取键、值、将值转换成字符串...）
        // 获取值
        RCLCPP_INFO(this->get_logger(), "robot name = %s", p1.as_string().c_str());
        RCLCPP_INFO(this->get_logger(), "robot height = %.2f", p2.as_double());
        RCLCPP_INFO(this->get_logger(), "robot wheel nums = %ld", p3.as_int());
        // 获取键
        RCLCPP_INFO(this->get_logger(), "p1's name = %s", p1.get_name().c_str());
        // 获取键的类型
        RCLCPP_INFO(this->get_logger(), "p1's name type = %s", p1.get_type_name().c_str());
        // 值转换为字符串
        RCLCPP_INFO(this->get_logger(), "p2's value to string = %s", p2.value_to_string().c_str());
    }
};

int main(int argc, char * argv[])
{
    // 2.初始化ROS2节点
    rclcpp::init(argc, argv);

    // 4.调用spin函数
    rclcpp::spin(std::make_shared<MyParam>());

    // 5.释放资源
    rclcpp::shutdown();
    return 0;
}