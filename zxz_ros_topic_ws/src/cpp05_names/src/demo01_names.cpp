/*
通过编码方式实现设置节点名称/命名空间, 解决节点重名
*/

#include <rclcpp/rclcpp.hpp>

class NodeCPP: public rclcpp::Node
{
public:
    NodeCPP()
    : Node("cpp_node", "ns_cpp")
    // Node(const rclcpp::Node &other, const std::string &sub_namespace)
    {
        
    }
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<NodeCPP>());

    rclcpp::shutdown();

    return 0;
}