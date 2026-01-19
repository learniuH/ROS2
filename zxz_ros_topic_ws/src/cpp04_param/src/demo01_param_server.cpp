/*
需求：创建参数服务端，并操作参数（增删改查）
流程：
    自定义节点类
    1.增
    2.查
    3.改
    4.删
*/

#include <rclcpp/rclcpp.hpp>

class ParamServer: public rclcpp::Node
{
public:
    ParamServer()
    : Node("param_server", rclcpp::NodeOptions().allow_undeclared_parameters(true))
    {
        RCLCPP_INFO(this->get_logger(), "参数服务端创建！");
    }
    // 1.增
    void declared_param()
    {
        RCLCPP_INFO(this->get_logger(), "--------------------增--------------------");
        this->declare_parameter("string_param", "LearniuH");
        this->declare_parameter("double_param", 1.78);
        this->declare_parameter("int_param", 18);

        // 也可以通过set_parameter实现添加参数，但前提为rclcpp::NodeOptions().allow_undeclared_parameters(true)
        this->set_parameter(rclcpp::Parameter("byte_param", 11));
    }
    // 2.查
    void get_param()
    {
        RCLCPP_INFO(this->get_logger(), "--------------------查--------------------");
        // 获取指定参数
        auto str_param = this->get_parameter("string_param");
        RCLCPP_INFO(this->get_logger(), "key = %s, value = %s", str_param.get_name().c_str(), str_param.as_string().c_str());

        // 获取一些参数
        // std::vector<rclcpp::Parameter, std::allocator<rclcpp::Parameter>> 
        // get_parameters(const std::vector<std::string> &names) const
        auto params = this->get_parameters({"string_param", "double_param", "int_param"});
        for (auto &&param : params)
        {
            RCLCPP_INFO(this->get_logger(), "%s = %s", param.get_name().c_str(), param.value_to_string().c_str());
        }

        // 判断是否包含
        RCLCPP_INFO(this->get_logger(), "是否包含double_param? %d", this->has_parameter("double_param"));
        RCLCPP_INFO(this->get_logger(), "是否包含float_param? %d", this->has_parameter("float_param"));
    }
    // 3.改
    void update_param()
    {
        RCLCPP_INFO(this->get_logger(), "--------------------改--------------------");
        this->set_parameter(rclcpp::Parameter("int_param", 24));
        RCLCPP_INFO(this->get_logger(), "int_param = %ld", this->get_parameter("int_param").as_int());
    }
    // 4.删
    void del_param()
    {
        RCLCPP_INFO(this->get_logger(), "--------------------删--------------------");
        // this->undeclare_parameter("string_param");  // 由 declared_parameter() 声明的参数不能被删除
        this->undeclare_parameter("byte_param");    // 可以删除未声明而设置的参数
        RCLCPP_INFO(this->get_logger(), "是否包含byte_param: %d", this->has_parameter("byte_param"));
    }
};

int main(int argc, char * argv[])
{
    // 2.初始化ROS2节点
    rclcpp::init(argc, argv);

    auto server = std::make_shared<ParamServer>();
    server->declared_param();
    server->get_param();
    server->update_param();
    server->del_param();
    // 4.调用spin函数
    rclcpp::spin(server);

    // 5.释放资源
    rclcpp::shutdown();
    return 0;
}