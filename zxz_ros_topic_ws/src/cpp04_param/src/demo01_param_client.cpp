/*
需求：创建参数客户端，查询或修改服务端参数
自定义节点类：
    1.创建参数客户端对象
    2.连接服务端
    3.参数查询
    4.修改参数
*/

#include <rclcpp/rclcpp.hpp>

using namespace std::chrono_literals;

class ParamClient: public rclcpp::Node
{
public:
    ParamClient()
    : Node("param_client")
    {
        RCLCPP_INFO(this->get_logger(), "参数客户端创建！");
        // 1. 创建参数客户端对象
        param_client_ = std::make_shared<rclcpp::SyncParametersClient>(this, "param_server");
        // 参数1.当前对象所依赖的节点   参数2.参数服务端节点名称
        /*
        问题：服务通信通过『服务话题』关联，那为什么参数客户端是通过参数服务端的节点的名称建立关联？
        答：
            1.参数服务端启动后，地层封装了多个服务通信的服务端
            2.每个服务端的话题都是采用 『/服务节点名称/xxxx』的方式命名，xxxx为 "list_parameters" "set_parameters"等
            3.参数客户端创建后，也会封装多个服务通信的客户端
            4.客户端与服务端建立关联，也要用的同的话题，因此客户端创建时需要传入服务端节点的名称
        */
    }
    // 2.连接服务端
    bool connect_server()
    {
        while (!param_client_->wait_for_service(1s))
        {
            if (!rclcpp::ok())
            {
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "客户取消连接！");
                return false;
            }
            RCLCPP_INFO(this->get_logger(), "服务连接中...");
        }
        return true;
    }
    // 3.参数查询
    void get_param()
    {
        RCLCPP_INFO(this->get_logger(), "--------------------参数查询--------------------");
        // 获取某个参数
        std::string string_param_value = param_client_->get_parameter<std::string>("string_param");
        RCLCPP_INFO(this->get_logger(), "string_param = %s", string_param_value.c_str());
        double double_param_value = param_client_->get_parameter<double>("double_param");
        RCLCPP_INFO(this->get_logger(), "double_param = %.2f", double_param_value);

        // 获取多个参数
        auto params = param_client_->get_parameters({"string_param", "double_param", "int_param"});
        for (auto &&param : params)
        {
            RCLCPP_INFO(this->get_logger(), "%s = %s", param.get_name().c_str(), param.value_to_string().c_str());
        }

        // 判断是否包含某个参数
        RCLCPP_INFO(this->get_logger(), "是否包含byte_param? %d", param_client_->has_parameter("byte_param"));
    }
    // 4.修改参数
    void update_param()
    {
        RCLCPP_INFO(this->get_logger(), "--------------------参数修改--------------------");
        param_client_->set_parameters({rclcpp::Parameter("int_param", 18), rclcpp::Parameter("byte_param", 2026)});
        // 可以设置先前不存在的参数，但前提是服务端设置了rclcpp::NodeOptions().allow_undeclared_parameters(true)
        RCLCPP_INFO(this->get_logger(), "新设置的参数byte_param = %d", param_client_->get_parameter<int>("byte_param"));
    }

private:
    rclcpp::SyncParametersClient::SharedPtr param_client_;
};

int main(int argc, char * argv[])
{
    // 2.初始化ROS2节点
    rclcpp::init(argc, argv);

    auto client = std::make_shared<ParamClient>();
    if (!client->connect_server())
    {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "参数服务端连接失败！程序退出！");
        return 0;
    }
    client->get_param();
    client->update_param();
    client->get_param();

    // 5.释放资源
    rclcpp::shutdown();
    return 0;
}