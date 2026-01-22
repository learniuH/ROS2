

#include <rclcpp/rclcpp.hpp>
#include <turtlesim/msg/pose.hpp>
#include <base_interfaces_demo/srv/distance.hpp>

using std::placeholders::_1;
using std::placeholders::_2;
using base_interfaces_demo::srv::Distance;

/* 
    测试：
    ros2 service call /get_distance base_interfaces_demo/srv/Distance "{'x': 5, 'y': 5, 'theta': 3.14}" 
*/

class DistanceServer: public rclcpp::Node
{
public:
    DistanceServer()
    : Node("distance_server")
    , x(0.0)
    , y(0.0)
    {
        RCLCPP_INFO(this->get_logger(), "服务端创建！");
        pose_sub_ = this->create_subscription<turtlesim::msg::Pose>("/turtle1/pose", 10, std::bind(&DistanceServer::pose_cb, this, _1));
        server_ = this->create_service<Distance>("get_distance", std::bind(&DistanceServer::proc_req, this, _1, _2));
    }
private:
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub_;
    rclcpp::Service<Distance>::SharedPtr server_;
    float x, y;
    void pose_cb(const turtlesim::msg::Pose & pose)
    {
        x = pose.x;
        y = pose.y;
    }
    void proc_req(const Distance::Request::SharedPtr req, Distance::Response::SharedPtr res)
    {
        float x_diff = x - req->x;
        float y_diff = y - req->y;
        float dis = std::sqrt(x_diff * x_diff + y_diff * y_diff);

        // 设置响应
        res->distance = dis;

        RCLCPP_INFO(this->get_logger(), "目标点坐标(%.2f, %.2f)\n原生乌龟坐标(%.2f, %.2f)\n二者距离：%.2f", req->x, req->y, x, y, dis);
    }
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    
    rclcpp::spin(std::make_shared<DistanceServer>());

    rclcpp::shutdown();
    
    return 0;
}