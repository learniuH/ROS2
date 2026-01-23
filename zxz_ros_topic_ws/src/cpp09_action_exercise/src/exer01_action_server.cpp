/*
需求：处理客户端发送的请求数据（目标点），控制乌龟向目标点运动，且要连续反馈剩余距离
流程：
    1.创建原生乌龟位姿订阅方，获取当前乌龟坐标
    2.创建速度指令发布方，控制乌龟运动
    3.创建一个动作服务端
    4.解析动作客户端提交的数据是否合法
    5.处理客户端的取消请求操作
    6.实现主逻辑（耗时操作，启动子线程）
    7.子线程中发布速度指令、产生连续反馈，并响应最终结果
*/

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <turtlesim/msg/pose.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <base_interfaces_demo/action/nav.hpp>

using base_interfaces_demo::action::Nav;
using std::placeholders::_1;
using std::placeholders::_2;

/*
    测试：
    ros2 action send_goal /nav_action base_interfaces_demo/action/Nav "{'goal_x': 6, 'goal_y': 7, 'goal_theta': 0}"
*/

class NavServer: public rclcpp::Node
{
public:
    NavServer()
    : Node("nav_server")
    , x(0)
    , y(0)
    , theta(0)
    {
        RCLCPP_INFO(this->get_logger(), "动作服务端创建！");
        // 1.创建一个订阅方（原生乌龟位姿 /turtle/pose）
        pose_sub_ = this->create_subscription<turtlesim::msg::Pose>("/turtle1/pose", 10, std::bind(&NavServer::pose_cb, this, _1));

        // 2.创建速度指令发布方，控制乌龟运动
        cmd_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
        
        // 3.创建一个动作服务端
        nav_server_ = rclcpp_action::create_server<Nav>(
            this,
            "nav_action",
            std::bind(&NavServer::handle_goal, this, _1, _2),
            std::bind(&NavServer::handle_cancel, this, _1),
            std::bind(&NavServer::handle_accepted, this, _1)
        );
        /*
            rclcpp_action::Server<ActionT>::SharedPtr 
            create_server<ActionT, NodeT>(
            NodeT node, 
            const std::string &name, 
            rclcpp_action::Server<ActionT>::GoalCallback handle_goal, 
            rclcpp_action::Server<ActionT>::CancelCallback handle_cancel, 
            rclcpp_action::Server<ActionT>::AcceptedCallback handle_accepted, 
            const rcl_action_server_options_t &options = rcl_action_server_get_default_options(), 
            rclcpp::CallbackGroup::SharedPtr group = nullptr)
        */
    }
private:
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub_;
    float x, y, theta;
    rclcpp_action::Server<Nav>::SharedPtr nav_server_;
    void pose_cb(const turtlesim::msg::Pose & pose)
    {
        x = pose.x;
        y = pose.y;
        theta = pose.theta;
    }
    /* std::function<GoalResponse(const GoalUUID &, std::shared_ptr<const typename ActionT::Goal>)>; */
    rclcpp_action::GoalResponse handle_goal(const rclcpp_action::GoalUUID & /* unused */, std::shared_ptr<const Nav::Goal> goal)
    {
        if (!goal)
        {
            RCLCPP_INFO(this->get_logger(), "goal is nullptr!");
            return rclcpp_action::GoalResponse::REJECT;
        }
        else
        {
            float goal_x = goal->goal_x;
            float goal_y = goal->goal_y;
            float goal_theta = goal->goal_theta;
            if (goal_x < 0.0 || goal_x > 11.08 || goal_y < 0 || goal_y > 11.08)
            {
                RCLCPP_INFO(this->get_logger(), "请求的目标点超过屏幕范围！");
                return rclcpp_action::GoalResponse::REJECT;
            }
            else
            {
                RCLCPP_INFO(this->get_logger(), "已收到目标请求：x = %.2f, y = %.2f, theta = %.2f", goal_x, goal_y, goal_theta);
                return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
            }
        }
    }
    /* std::function<CancelResponse(std::shared_ptr<ServerGoalHandle<ActionT>>)>; */
    rclcpp_action::CancelResponse handle_cancel(std::shared_ptr<rclcpp_action::ServerGoalHandle<Nav>> /* unused */)
    {
        RCLCPP_INFO(this->get_logger(), "收到任务取消请求！");
        return rclcpp_action::CancelResponse::ACCEPT;
    }
    void execute(std::shared_ptr<rclcpp_action::ServerGoalHandle<Nav>> goal_handle)
    {
        auto feedback = std::make_shared<Nav::Feedback>();
        auto result = std::make_shared<Nav::Result>();
        geometry_msgs::msg::Twist twist;

        float goal_x = goal_handle->get_goal()->goal_x;
        float goal_y = goal_handle->get_goal()->goal_y;
        float scale = 0.5;  // 比例控制
        rclcpp::Rate rate(10.0);
        while (true)
        {
            // 如果要取消任务，则需要特殊处理
            if (goal_handle->is_canceling())
            {
                result->turtle_x = x;
                result->turtle_y = y;
                result->turtle_theta = theta;
                goal_handle->canceled(result);
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "用户中止！");
                return;
            }

            // 6-2.发布乌龟运动指令
            // 计算剩余距离并发布
            float distance_x = goal_x - x;
            float distance_y = goal_y - y;
            float distance = std::sqrt(distance_x * distance_x + distance_y * distance_y);
            feedback->distance = distance;
            goal_handle->publish_feedback(feedback);

            if (distance < 0.05)
            {
                break;
            }
            else 
            {
                // 根据剩余距离计算速度指令并发布
                twist.linear.x = distance_x * scale;
                twist.linear.y = distance_y * scale;
                cmd_pub_->publish(twist);

                RCLCPP_INFO(this->get_logger(), "乌龟当前坐标：(%.2f, %.2f)，距离目标：%.2f", x, y, distance);
            }
            rate.sleep();
        }

        if (rclcpp::ok())
        {
            result->turtle_x = x;
            result->turtle_y = y;
            result->turtle_theta = theta;
            goal_handle->succeed(result);
            RCLCPP_INFO(this->get_logger(), "乌龟导航结束，当前坐标：(%.2f, %.2f)", x, y);
        }
    }
    /* std::function<void (std::shared_ptr<ServerGoalHandle<ActionT>>)>; */
    void handle_accepted(std::shared_ptr<rclcpp_action::ServerGoalHandle<Nav>> goal_handle)
    {
        std::thread(std::bind(&NavServer::execute, this, goal_handle)).detach();
    }
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<NavServer>());

    rclcpp::shutdown();

    return 0;
}