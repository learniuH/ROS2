/*
    问题：Time 与 Duration 有什么区别？
    答：
        1.二者只是API使用类似；
        2.二者有着本质区别；
            rclcpp::Time time2(2, 500000000L); -- 指的是具体时刻 -- 1970-01-01 00::00::02::500
            rclcpp::Duration du2(2, 500000000L); -- 指的是一个时间段，持续2.5s
*/

#include <rclcpp/rclcpp.hpp>

using namespace std::chrono_literals;

class TimeNode: public rclcpp::Node
{
public:
    TimeNode()
    : Node("time_node")
    {
        // this->demo_rate();
        // this->demo_time();
        // this->demo_duration();
        
        this->demo_opt();
    }
private:
    // 演示Rate使用
    void demo_rate()
    {
        // 1.创建Rate对象
        rclcpp::Rate rate1(500ms);   // 间隔
        rclcpp::Rate rate2(1.0);     // 频率

        // 2.调用Rate的sleep函数
        while (rclcpp::ok())
        {
            RCLCPP_INFO(this->get_logger(), "---------------");
            rate2.sleep();
        }
    }
    // 演示Time使用
    void demo_time()
    {
        // 1.创建Time对象
        /* Time(int64_t nanoseconds = 0L, rcl_clock_type_t clock_type = RCL_SYSTEM_TIME) */
        rclcpp::Time time1(500000000L);     // 0.5s（1s = 1000000000ns）

        /* Time(int32_t seconds, uint32_t nanoseconds, rcl_clock_type_t clock_type = RCL_SYSTEM_TIME) */
        rclcpp::Time time2(2, 500000000L);  // 2.5s

        // rclcpp::Time time_now = this->get_clock()->now();
        rclcpp::Time time_now = this->now();

        // 2.调用Time对象的函数
        RCLCPP_INFO(this->get_logger(), "sec = %.2f, nsec = %ld", time1.seconds(), time1.nanoseconds());
        RCLCPP_INFO(this->get_logger(), "sec = %.2f, nsec = %ld", time2.seconds(), time2.nanoseconds());
        RCLCPP_INFO(this->get_logger(), "sec = %.2f, nsec = %ld", time_now.seconds(), time_now.nanoseconds());
    }
    // 演示Duration使用
    void demo_duration()
    {
        // 1.创建Duration对象
        rclcpp::Duration du1(0.5s);
        /* Duration(std::chrono::nanoseconds nanoseconds) */ 

        rclcpp::Duration du2(2, 500000000L);
        /* Duration(int32_t seconds, uint32_t nanoseconds) */

        // 2.调用Duration对象的函数
        RCLCPP_INFO(this->get_logger(), "sec = %.2f, nsec = %ld", du1.seconds(), du1.nanoseconds());
        RCLCPP_INFO(this->get_logger(), "sec = %.2f, nsec = %ld", du2.seconds(), du2.nanoseconds());
    }
    // 演示运算符使用
    void demo_opt()
    {
        rclcpp::Time t1(10, 0);
        rclcpp::Time t2(30, 0);

        rclcpp::Duration du1(8, 0);
        rclcpp::Duration du2(17, 0);

        // 运算符
        // 比较运算
        RCLCPP_INFO(this->get_logger(), "t1 >= t2 ? %d", t1 >= t2);     // 0
        RCLCPP_INFO(this->get_logger(), "t1 <= t2 ? %d", t1 <= t2);     // 1
        RCLCPP_INFO(this->get_logger(), "du1 > du2 ? %d", du1 > du2);   // 0

        // 数学运算
        rclcpp::Duration du3 = t1 - t2; // 不支持 Time + Time（时间点 + 时间点）
        rclcpp::Time t3 = t2 + du1;
        rclcpp::Duration du4 = du2 * 3;
        RCLCPP_INFO(this->get_logger(), "t1 - t2 = %.2f", du3.seconds());   // -20
        RCLCPP_INFO(this->get_logger(), "t2 + du1 = %.2f", t3.seconds());   // 38
        RCLCPP_INFO(this->get_logger(), "du2 * 3 = %.2f", du4.seconds());   // 51
    }
};

int main(int argc, char * argv[])
{
    // 初始化ROS2节点
    rclcpp::init(argc, argv);

    // 调用spin函数
    rclcpp::spin(std::make_shared<TimeNode>());

    // 释放资源
    rclcpp::shutdown();

    return 0;
}