/*
需求：向动作服务端发送目标点数据，并处理响应结果
流程：
    1.解析launch文件传入的参数
    2.自定义节点类
        2-1.创建动作客户端
        2-2.连接服务端，发送请求
        2-3.处理目标值相关响应结果
        2-4.处理连续反馈
        2-5.处理最终响应
*/

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <base_interfaces_demo/action/nav.hpp>

using base_interfaces_demo::action::Nav;
using std::placeholders::_1;
using std::placeholders::_2;
using namespace std::chrono_literals;

class NavClient: public rclcpp::Node
{
public:
    NavClient()
    : Node("nav_client")
    {
        RCLCPP_INFO(this->get_logger(), "动作客户端创建！");
        // 2-1.创建动作客户端
        nav_client_ = rclcpp_action::create_client<Nav>(this, "nav_action");
        /*
            rclcpp_action::Client<ActionT>::SharedPtr 
            create_client<ActionT, NodeT>(
            NodeT node, 
            const std::string &name, 
            rclcpp::CallbackGroup::SharedPtr group = nullptr, 
            const rcl_action_client_options_t &options = rcl_action_client_get_default_options())
        */
    }
    void send_goal(float goal_x, float goal_y, float goal_theta)
    {
        // 2-2.连接服务端
        if (!nav_client_->wait_for_action_server(10s))
        {
            RCLCPP_INFO(this->get_logger(), "服务连接超时！");
            return;
        }

        Nav::Goal goal;
        goal.goal_x = goal_x;
        goal.goal_y = goal_y;
        goal.goal_theta = goal_theta;

        rclcpp_action::Client<Nav>::SendGoalOptions options;
        options.goal_response_callback = std::bind(&NavClient::goal_response_callback, this, _1);

        options.feedback_callback = std::bind(&NavClient::feedback_callback, this, _1, _2);

        options.result_callback = std::bind(&NavClient::result_callback, this, _1);

        auto future = nav_client_->async_send_goal(goal, options);
        /*
            std::shared_future<rclcpp_action::ClientGoalHandle<base_interfaces_demo::action::Nav>::SharedPtr> 
            async_send_goal(const base_interfaces_demo::action::Nav::Goal &goal, 
            const rclcpp_action::Client<base_interfaces_demo::action::Nav>::SendGoalOptions &options)
        */
    }
private:
    rclcpp_action::Client<Nav>::SharedPtr nav_client_;
    /* std::function<void (typename GoalHandle::SharedPtr)>; */
    void goal_response_callback(rclcpp_action::ClientGoalHandle<Nav>::SharedPtr goal_handle)
    {
        if (!goal_handle)
        {
            RCLCPP_INFO(this->get_logger(), "目标请求被服务器拒绝！");
        }
        else
        {
            RCLCPP_INFO(this->get_logger(), "目标处理中！");
        }
    }
    /* std::function<void (typename ClientGoalHandle<ActionT>::SharedPtr, const std::shared_ptr<const Feedback>)>; */
    void feedback_callback(rclcpp_action::ClientGoalHandle<Nav>::SharedPtr /* unused */, const std::shared_ptr<const Nav::Feedback> feedback)
    {
        RCLCPP_INFO(this->get_logger(), "乌龟正向目标移动，距离：%.2f", feedback->distance);
    }
    /* std::function<void (const WrappedResult & result)> */
    void result_callback(const rclcpp_action::ClientGoalHandle<Nav>::WrappedResult & result)
    {
        if (result.code == rclcpp_action::ResultCode::SUCCEEDED)
        {
            RCLCPP_INFO(this->get_logger(), "乌龟最终位姿信息：(%.2f, %.2f, %.2f)", result.result->turtle_x, result.result->turtle_y, result.result->turtle_theta);
        }
        else 
        {
            RCLCPP_WARN(this->get_logger(), "服务器响应失败！");
        }
    }
};

int main(int argc, char * argv[])
{
    if (argc != 5)
    {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "请提交x坐标、y坐标、航向角theta");
        return 1;
    }
    rclcpp::init(argc, argv);

    auto nav_client = std::make_shared<NavClient>();
    nav_client->send_goal(atof(argv[1]), atof(argv[2]), atof(argv[3]));

    rclcpp::spin(nav_client);

    rclcpp::shutdown();

    return 0;
}