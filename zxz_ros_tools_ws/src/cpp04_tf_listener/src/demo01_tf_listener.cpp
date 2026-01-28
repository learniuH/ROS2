/*
    需求：先发布laser到base_link的坐标相对关系，再发布camera到base_link的坐标相对关系
         求解laser到camera的坐标系相对关系
    流程：
        1.包含头文件
        2.初始化ROS2客户端
        3.自定义节点类
            3.1.创建缓存对象，融合多个坐标系相对关系为一颗坐标树
            3.2.创建监听器，绑定缓存对象，会将所有广播器的数据写入缓存
            3.3.实现转换
        4.调用spin函数，并传入对象节点指针
        5.释放资源
*/

#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>

using namespace std::chrono_literals;

class TFListener: public rclcpp::Node
{
public:
    TFListener()
    : Node("tf_listener")
    {
        RCLCPP_INFO(this->get_logger(), "TF监听对象已创建！");
        // 创建缓存对象，融合多个坐标系相对关系为一颗坐标树
        buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
        // 创建监听器，绑定缓存对象，会将所有广播器广播的数据写入缓存
        listener_ = std::make_shared<tf2_ros::TransformListener>(*buffer_, this);
        // 编写定时器，实现循环转换
        timer_ = this->create_wall_timer(1s, std::bind(&TFListener::on_timer, this));
    }
private:
    std::unique_ptr<tf2_ros::Buffer> buffer_;
    std::shared_ptr<tf2_ros::TransformListener> listener_;
    rclcpp::TimerBase::SharedPtr timer_;
    void on_timer()
    {
        /*
            geometry_msgs::msg::TransformStamped lookupTransform(
            const std::string &target_frame,    // 父坐标系
            const std::string &source_frame,    // 子坐标系
            const tf2::TimePoint &time)         // 转换时间点
        */
        /** \brief Get the transform between two frames by frame ID.
         * \param target_frame The frame to which data should be transformed
         * \param source_frame The frame where the data originated
         * \param time The time at which the value of the transform is desired. (0 will get the latest)
         * \return The transform between the frames
         *
         * Possible exceptions tf2::LookupException, tf2::ConnectivityException,
         * tf2::ExtrapolationException, tf2::InvalidArgumentException
         */
        try
        {
            // 实现坐标转换
            auto transform_stamped = buffer_->lookupTransform("camera", "laser", tf2::TimePointZero);
            RCLCPP_INFO(this->get_logger(), "----------转换结果---------");
            RCLCPP_INFO(this->get_logger(), "frame_id: %s", transform_stamped.header.frame_id.c_str());
            RCLCPP_INFO(this->get_logger(), "child_frmae_id: %s", transform_stamped.child_frame_id.c_str());
            RCLCPP_INFO(this->get_logger(), "坐标：(%.2f, %.2f, %.2f)", 
                transform_stamped.transform.translation.x, 
                transform_stamped.transform.translation.y, 
                transform_stamped.transform.translation.z
            );
        }
        catch(const tf2::LookupException & e)
        {
            RCLCPP_WARN(this->get_logger(), "坐标变换异常！%s", e.what());
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