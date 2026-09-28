# CURRENT

最后更新：2026-09-28

## 当前主线

ROS2 第四章课程已经跟完，当前主线切换为 **独立综合项目 Turtle Patrol System**，目标是把 Topic、Service、Parameter、Launch、namespace 和调试能力从“跟随教学能完成”提升到“能从空工程独立组织并验证”。

当前独立工作区：`~/my_ros/my_test`

当前包：
- `turtle_patrol_interface`：自定义 `srv/SetTarget.srv`
- `turtle_patrol`：`TurtleController` 功能包

完整当日过程见 [2026-09-28 学习记录](daily/2026-09-28.md)。

## 今天已经有运行证据的内容

- Parameter：声明、读取、动态 callback、合法性拒绝、集成控制器、`AsyncParametersClient` 正常/失败结果处理均已跑过。
- Launch：参数传入、double/int 类型问题、Node name、namespace、remapping 已实战；两套 turtlesim 通过 `robot1/robot2` namespace + 相对 Topic 实现隔离。
- 独立项目接口包 `turtle_patrol_interface` 已成功构建。
- `turtle_patrol` 最小 SetTarget Service Server 已完成 Node/create_service/callback/main 基本结构。
- 功能包已加入相对 Topic `turtle1/pose` 的 Pose Subscriber；最后截图显示 `turtle_controller` 持续收到 `x≈5.54, y≈5.54, theta≈0.00`，Subscriber 链路有运行证据。
- workspace 内自定义接口依赖曾因 package.xml 缺少 `turtle_patrol_interface` 导致 colcon 同时构建两个包；补依赖后该构建顺序问题已解决。

## 今天暴露的能力缺口

- Service/main 固定 API 骨架曾遗忘：`create_service`、Service SharedPtr、`init → make_shared → spin → shutdown` 需要提示恢复。
- `shared_ptr` 创建语法一度混淆；已复习 unique/shared/weak 所有权模型，但仍需延迟复现。
- async/future 与 `wait_for_service` 语义在闭卷复测中答得不完整，纠正后暂不升独立等级。
- 目标状态设计时曾漏 Response 失败字段、漏 `has_target_=true`，说明“Request → 持久成员状态”的状态迁移还需继续训练。
- CMake 固定语法和依赖链仍易出错：`REQUIRED`、`ament_target_dependencies`、`install(...)`、`turtlesim` target dependency、消息头路径均出现过错误。
- `CMakeLists.txt` 与 `package.xml` 的职责通过真实报错刚建立，需要之后再次独立配置验证。

## 当前状态边界

第四章“课程已完成”只表示内容已经跟过并有多项实战成功，不等于通过 TEACHING_PROTOCOL 的独立验收。

Service / Parameter / Launch 目前仍以 guided/半独立 L2 为主；在 Turtle Patrol 项目完成从空工程实现、变式、故意失败、CLI 诊断和延迟复现之前，不直接记为 L3。

截图中功能包 `package.xml` 已看到 `turtle_patrol_interface` 依赖；最终项目整理时还要确认 `rclcpp`、`turtlesim` 等直接依赖也完整写入 manifest，而不是只依赖本机环境能够 build。

## 下一次开始点

从当前 Turtle Patrol 项目继续，不重新从课程示例开始：

1. 核验合法 SetTarget 请求后确实执行 `has_target_=true`，并用正常/非法请求验证旧目标不被非法请求覆盖。
2. 补齐/核验 `package.xml` 中 `rclcpp`、`turtlesim`、`turtle_patrol_interface` 直接依赖。
3. 加 `geometry_msgs::msg::Twist` Publisher，Topic 使用相对名称 `turtle1/cmd_vel`。
4. 在 Pose callback 中实现 distance、`atan2`、角度误差归一化、转向阈值、线速度/角速度输出和到点停止。
5. 再接回 Parameter、goal_client、parameter_client、单系统 Launch 与双 namespace Launch。
6. 做服务缺席、非法目标、非法参数、Topic 冲突、Launch 参数类型错误等故意失败测试。
7. 最后从空文件延迟复现 Service/main/CMake 核心骨架，再评是否达到独立 L3。

## 仍需并行保留的历史待验证

旧的 Linux Socket/TCP、producer-consumer/rwlock、Qt 主线程更新、speech_thread、9 月 25 日闭环草稿等遗留项继续保留在此前 daily/CURRENT 历史记录中；当前 ROS2 独立项目优先推进，不用历史遗留阻断主线。
