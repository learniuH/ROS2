#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

class NodeCPP: public rclcpp::Node
{
public:
    NodeCPP()
    /*
    通过编码方式实现设置节点名称/命名空间, 解决节点重名
    */
    : Node("node_name", "namespace")
    // Node(const rclcpp::Node &other, const std::string &sub_namespace)
    {
        // 全局话题：/global_topic
        global_pub_ = this->create_publisher<std_msgs::msg::String>("/global_topic", 10);

        // 相对话题：/namespace/relative_topic
        relative_pub_ = this->create_publisher<std_msgs::msg::String>("relative_topic", 10);

        // 私有话题：/namespace/node_name/private_topic
        private_pub_ = this->create_publisher<std_msgs::msg::String>("~/private_topic", 10);
    }
private:
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr global_pub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr relative_pub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr private_pub_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<NodeCPP>());

    rclcpp::shutdown();

    return 0;
}