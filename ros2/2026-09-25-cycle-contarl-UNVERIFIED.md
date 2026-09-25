# CycleContarl 学习快照 — UNVERIFIED / 未验证

日期：2026-09-25。来源：学习者在「检索并制定学习内容」最后一次贴出的代码。仅还原聊天转义和空格，保留原命名、逻辑与问题。此文件为学习记录，不是可运行示例或构建目标；尚未进行 colcon 编译/运行验证。

明日首先检查：`cmd_vel.linear.y` 应改为转向通道 `cmd_vel.angular.z`；`max_speed_` 尚未用于限幅；`target_ang_` 可改为 callback 局部变量；确认 `<cmath>` 与 `M_PI` 可用性（`M_PI` 不是标准 C++ 保证的宏）。未使用的 `current_theta` 和头文件可随后整理。

完整进度与验证顺序见 [当日记录](../daily/2026-09-25.md)。以下保留错误，不能据此认定闭环控制已跑通。

```cpp
#include "rclcpp/rclcpp.hpp"

#include "turtlesim/msg/pose.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "chrono"

#include <iostream>
#include <memory>

class CycleContarl : public rclcpp::Node
{
    public:
        CycleContarl() : Node("CycleContarl"){
            subcription_ = this->create_subscription<turtlesim::msg::Pose>(
                "/turtle1/pose",    // topic
                10,
                [this](turtlesim::msg::Pose::ConstSharedPtr msg)
                {
                    pose_callback(msg);
                }
            );

            publisher_ = this->create_publisher<geometry_msgs::msg::Twist>(
                "/turtle1/cmd_vel",
                10
            );
        }


    private:
        void pose_callback(turtlesim::msg::Pose::ConstSharedPtr msg)
        {
            double current_x = msg->x;
            double current_y = msg->y;
            double current_theta = msg->theta;

            RCLCPP_INFO(this->get_logger(),
            "x = %.2f, y = %.2f, theta = %.2f \n",
            msg->x,
            msg->y,
            msg->theta);

            double error_x = target_x_ - current_x;
            double error_y = target_y_ - current_y;
            target_ang_ = std::atan2(error_y, error_x);
            double error_ang = target_ang_ - msg->theta;
            // calculate error

            double error_dis = std::sqrt(error_x * error_x + error_y * error_y);
            // handing angle issus
            while(error_ang > M_PI)
            {
                error_ang -= 2.0 * M_PI;
            }
            while (error_ang < -M_PI)
            {
                error_ang += 2.0 * M_PI;
            }
            //creat speed message
            geometry_msgs::msg::Twist cmd_vel;

            cmd_vel.linear.x = k_dis * error_dis;
            cmd_vel.linear.y = k_ang * error_ang;

            //send speed
            publisher_->publish(cmd_vel);
        }

    private:
        rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr subcription_;
        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;

        double target_x_{1.0};
        double target_y_{1.0};
        double target_ang_{1.0};

        double k_dis{1.0};
        double k_ang{1.0};
        double max_speed_{3.0};

};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<CycleContarl>();
    RCLCPP_INFO(node->get_logger(), "Node has been started.");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
```
