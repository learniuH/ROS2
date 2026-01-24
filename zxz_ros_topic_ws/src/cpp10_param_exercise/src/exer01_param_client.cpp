/*
需求：修改turtlesim_node的背景颜色
流程：
    1.创建参数客户端
    2.连接参数服务端
    3.修改参数
*/

#include <rclcpp/rclcpp.hpp>

using namespace std::chrono_literals;

class RGBParamClient: public rclcpp::Node
{
public:
    RGBParamClient()
    : Node("rgb_param_client")
    {
        RCLCPP_INFO(this->get_logger(), "参数客户端创建！");

        // 1.创建参数客户端
        rgb_param_client_ = std::make_shared<rclcpp::SyncParametersClient>(this, "/turtlesim");
    }
    // 2.连接参数服务端
    bool connect_server()
    {
        while (!rgb_param_client_->wait_for_service(1s))
        {
            if (rclcpp::ok())
            {
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "用户取消，连接中止！");
                return false;
            }
            RCLCPP_INFO(this->get_logger(), "参数服务连接中！");
        }
        return true;
    }
    void update_background_r()
    {
        // 背景色渐变
        // 3-1.获取参数
        int background_r = rgb_param_client_->get_parameter<int>("background_r");
        int count = background_r;

        rclcpp::Rate rate(30.0);
        while (rclcpp::ok())
        {
            count <= 255 ? background_r += 2 : background_r -= 2;
            count <= 510 ? count += 2 : count = 0;

            // 修改服务端参数
            rgb_param_client_->set_parameters({rclcpp::Parameter("background_r", background_r)});

            rate.sleep();
        }
        
    }
private:
    rclcpp::SyncParametersClient::SharedPtr rgb_param_client_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto client = std::make_shared<RGBParamClient>();
    if (!client->connect_server())
    {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "参数服务连接失败！");
        return 1;
    }
    client->update_background_r();

    rclcpp::shutdown();

    return 0;
}