/*
需求：客户端需要提交目标点坐标，并解析响应结果
流程：
    1.解析动态传入的数据，作为目标点坐标
    2.自定义节点类
        2-1.构造函数创建客户端
        2-2.客户端连接服务端
        2-3.发送请求数据
    3.调用节点对象指针的相关函数
*/

#include <rclcpp/rclcpp.hpp>
#include <base_interfaces_demo/srv/distance.hpp>

using base_interfaces_demo::srv::Distance;
using namespace std::chrono_literals;

class DistanceClient: public rclcpp::Node
{
public:
    DistanceClient()
    : Node("distance_client")
    {
        RCLCPP_INFO(this->get_logger(), "客户端创建！");
        client_ = this->create_client<Distance>("get_distance");
    }
    // 2-2.客户端连接服务端
    bool connect_server()
    {
        while (!client_->wait_for_service(1s))
        {
            if (!rclcpp::ok())
            {
                RCLCPP_INFO(this->get_logger(), "用户中止！");
                return false;
            }
            RCLCPP_INFO(this->get_logger(), "服务连接中...");
        }
        return true;
    }
    // 2-3.发送请求数据
    rclcpp::Client<Distance>::FutureAndRequestId send_request(float x, float y, float theta)
    {
        // Distance::Request::SharedPtr req;
        auto req = std::make_shared<Distance::Request>();
        req->x = x;
        req->y = y;
        req->theta = theta;
        /* 
            rclcpp::Client<base_interfaces_demo::srv::Distance>::FutureAndRequestId 
            async_send_request(std::shared_ptr<base_interfaces_demo::srv::Distance_Request> request) 
        */
        return client_->async_send_request(req);
    }
private:
    rclcpp::Client<Distance>::SharedPtr client_;
};

int main(int argc, char * argv[])
{
    if (argc != 5)
    {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "请提交x坐标、y坐标、航向角theta");
        return 1;
    }
    float x = atof(argv[1]);
    float y = atof(argv[2]);
    float theta = atof(argv[3]);

    rclcpp::init(argc, argv);

    auto client = std::make_shared<DistanceClient>();

    if (!client->connect_server())
    {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "服务连接失败");
        return 1;
    }

    // 发送请求并处理响应
    auto future = client->send_request(x, y, theta);

    if (rclcpp::spin_until_future_complete(client, future) == rclcpp::FutureReturnCode::SUCCESS)
    {
        RCLCPP_INFO(client->get_logger(), "两只乌龟间距离为%.2f", future.get()->distance);
    }
    else
    {
        RCLCPP_ERROR(client->get_logger(), "服务响应失败");
    }

    rclcpp::shutdown();

    return 0;
}