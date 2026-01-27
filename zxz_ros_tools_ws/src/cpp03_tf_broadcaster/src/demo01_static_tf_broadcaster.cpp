/*
    需求：编写静态坐标变换程序，执行时传入两个坐标系的相对位姿关系以及父子级坐标系id，
         程序运行发布静态坐标变换
         ros2 run 包名 可执行程序名 frame-id child-frame-id x y z roll pitch yaw
    流程：
         0.判断传入的参数是否合法
         1.创建广播对象
         2.组织并发布数据
*/

#define FRAME_ID        1
#define CHILD_FRMAE_ID  2
#define X               3
#define Y               4
#define Z               5
#define ROLL            6
#define PITCH           7
#define YAW             8

#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/static_transform_broadcaster.h>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <tf2/LinearMath/Quaternion.hpp>

class StaticTFBroadcaster: public rclcpp::Node
{
public:
    StaticTFBroadcaster(char * argv[])
    : Node("static_tf_broadcaster")
    {
        // 1.创建广播对象
        broadcaster_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);
        // 2.组织并发布数据
        pub_static_tf(argv);
    }
private:
    std::shared_ptr<tf2_ros::StaticTransformBroadcaster> broadcaster_;
    void pub_static_tf(char * argv[])
    {
        // 2-1.组织消息
        geometry_msgs::msg::TransformStamped transform;
        transform.header.stamp = this->now();               // 时间戳
        transform.header.frame_id = argv[FRAME_ID];         // 夫级坐标系
        transform.child_frame_id = argv[CHILD_FRMAE_ID];    // 子级坐标系
        // 设置偏移量
        transform.transform.translation.x = atof(argv[X]);
        transform.transform.translation.y = atof(argv[Y]);
        transform.transform.translation.z = atof(argv[Z]);
        // 设置四元数
        tf2::Quaternion qtn;
        // 将欧拉角转换成四元数
        qtn.setRPY(atof(argv[ROLL]), atof(argv[PITCH]), atof(argv[YAW]));

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
    for (int i = 0; i < argc; i++)
    {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "第%d个参数：%s", i, argv[i]);
    }
    if (argc != 9 && argc != 10)
    {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "%d：传入参数不合法！", argc);
    }

    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<StaticTFBroadcaster>(argv));

    rclcpp::shutdown();

    return 0;
}