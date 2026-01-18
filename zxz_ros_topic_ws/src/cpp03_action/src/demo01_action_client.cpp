/*
需求:编写动作客户端,可以发送一个整型数据到服务端,并处理服务端的连续反馈、最终响应结果
流程:
    前提:可以解析终端动态传入的参数
    3.自定义节点类
        3-1.创建动作客户端
        3-2.发送请求
        3-3.处理关于目标值的服务端响应(回调函数)
        3-4.处理连续反馈(回调函数)
        3-5.处理最终响应结果(回调函数)
*/

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <base_interfaces_demo/action/progress.hpp>

using base_interfaces_demo::action::Progress;
using namespace std::chrono_literals;
using std::placeholders::_1;
using std::placeholders::_2;

class ProgressActionClient: public rclcpp::Node
{
public:
    ProgressActionClient()
    : Node("progress_action_client")
    {
        // 3-1.动作客户端创建
        client_ = rclcpp_action::create_client<Progress>(this, "get_sum");
        /*
        rclcpp_action::Client<ActionT>::SharedPtr
        create_client<ActionT, NodeT>(
        NodeT node, 
        const std::string &name, 
        rclcpp::CallbackGroup::SharedPtr group = nullptr, 
        const rcl_action_client_options_t &options = rcl_action_client_get_default_options())
        */
        RCLCPP_INFO(this->get_logger(), "action 客户端创建！");
    }
    void send_goal(int num)
    {
        // 3-2-1.需要连接到服务端
        if (!client_->wait_for_action_server(10s))
        {
            RCLCPP_ERROR(this->get_logger(), "服务器连接失败！");
            return;
        }
        // 3-2-2.发送具体请求
        Progress::Goal goal;
        rclcpp_action::Client<Progress>::SendGoalOptions options;
        goal.num = num;
        options.goal_response_callback = std::bind(&ProgressActionClient::goal_response_callback, this, _1);
        options.feedback_callback = std::bind(&ProgressActionClient::feedback_callback, this, _1, _2);
        options.result_callback = std::bind(&ProgressActionClient::result_callback, this, _1);
        auto future = client_->async_send_goal(goal, options);
        /*
        std::shared_future<rclcpp_action::ClientGoalHandle<base_interfaces_demo::action::Progress>::SharedPtr> 
        async_send_goal(const base_interfaces_demo::action::Progress::Goal &goal, 
        const rclcpp_action::Client<base_interfaces_demo::action::Progress>::SendGoalOptions &options)
        */
    }
    // 3-3.处理关于目标值的服务端响应(回调函数)
    /*
    using GoalHandle = ClientGoalHandle<ActionT>;

    std::function<void (typename GoalHandle::SharedPtr)>
    */
    void goal_response_callback(rclcpp_action::ClientGoalHandle<Progress>::SharedPtr goal_handle)
    {
        // 如果请求被服务器拒绝，goal_handle 为空指针
        if (!goal_handle)
        {
            RCLCPP_INFO(this->get_logger(), "目标请求被服务端拒绝！");
        }
        else
        {
            RCLCPP_INFO(this->get_logger(), "目标处理中！");
        }
    }
    // 3-4.处理连续反馈(回调函数)
    /*
    std::function<void (typename ClientGoalHandle<ActionT>::SharedPtr, const std::shared_ptr<const Feedback>)>;
    */
    void feedback_callback(rclcpp_action::ClientGoalHandle<Progress>::SharedPtr /* unused */, const std::shared_ptr<const Progress::Feedback> feedback)
    {
        double progress = feedback->progress;
        RCLCPP_INFO(this->get_logger(), "当前进度：%d%%", (int)(progress * 100));
    }
    // 3-5.处理最终响应结果(回调函数)
    /*
    std::function<void (const WrappedResult & result)>
    */
    void result_callback(const rclcpp_action::ClientGoalHandle<Progress>::WrappedResult & result)
    {
        // 通过状态码判断最终状态
        if (result.code == rclcpp_action::ResultCode::SUCCEEDED)
        {
            RCLCPP_INFO(this->get_logger(), "最终结果为:%d", result.result->sum);
        }
        else if (result.code == rclcpp_action::ResultCode::ABORTED)
        {
            RCLCPP_INFO(this->get_logger(), "被中断！");
        }
        else if (result.code == rclcpp_action::ResultCode::ABORTED)
        {
            RCLCPP_INFO(this->get_logger(), "被取消！");
        }
        else
        {
            RCLCPP_INFO(this->get_logger(), "出现未知异常！");
        }
    }
private:
    rclcpp_action::Client<Progress>::SharedPtr client_;
};

int main(int argc, char * argv[])
{
    if (argc != 2)
    {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "请输入一个整型数据！");
        return 1;
    }
    // 2.初始化 ROS2 节点
    rclcpp::init(argc, argv);

    auto client = std::make_shared<ProgressActionClient>();
    // 3-2.发送请求
    client->send_goal(atoi(argv[1]));

    // 4.调用spin函数
    rclcpp::spin(client);

    // 5.释放资源
    rclcpp::shutdown();

    return 0;
}