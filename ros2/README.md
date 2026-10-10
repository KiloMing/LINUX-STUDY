# ROS 2

## 用途

保存 ROS 2 通信、调试、TF2、URDF、RViz、仿真、ros2_control 和传感器集成练习。

## 前置

Socket 基础、现代 CMake 与必要的 C++ 能力；实际 Ubuntu/ROS 版本必须先记录到 `ENVIRONMENT.md`。

## 当前证据

2026-09-21 已在本地创建 `ament_cmake` C++ package，开始练习 `rclcpp` 依赖、`add_executable`、`ament_target_dependencies`、`install(TARGETS ...)`、`ament_package()`、`colcon build`、`source install/setup.bash`、`ros2 pkg executables` 和 `ros2 run`。当前 package 已可被 ROS2 发现，并能查询已安装 executable；曾因 target 名不一致出现 `No executable found`。这属于 guided 入门证据，ROS2 构建基础记 L1；尚无独立 node/topic 数据流，当时 TF2/URDF/RViz 尚未开始；后续课程进度见下文，独立掌握仍按验收记录。

2026-09-23 已完成 `time_topic` 定时发布（学习者自述），并开始 Subscription。新增 Timer 句柄与创建、lambda 捕获、Executor 调度以及 CMake target 未定义问题的记录，见 [当日笔记](../daily/2026-09-23.md)。

2026-09-24 已在真实工作区完成 Timer Publisher → Topic → Subscription 联调：package 构建成功，`timer`/`subscription` executable 可查询并实际持续收发。随后用 C++ 接入 espeak-ng 完成 Topic 文本语音输出，并真实排查 `undefined reference`、普通 library 链接及 `target_link_libraries` signature 冲突。当前正把语音播放从 Subscription callback 解耦到 queue + `speech_thread`；线程版尚待最终运行验证。Topic 基础记 L2，详见 [当日笔记](../daily/2026-09-24.md)。

2026-09-25 学习者反馈 `demo_cpp_topic` 画圆发布与 Pose 打印成功；同一 Node 的闭环 `CycleContarl` 草稿尚未编译/运行，Topic 保持 L2。见 [今日记录](../daily/2026-09-25.md) 与 [UNVERIFIED 快照](2026-09-25-cycle-contarl-UNVERIFIED.md)。

2026-09-26 自定义 SystemStatus → Python 发布 → C++/Qt 显示链路按学习记录已跑通，闭环学习推进到阈值和限幅。当前停在第 4 章 4.1.1 Service；本次未重新构建验证，见 [今日记录](../daily/2026-09-26.md)。

2026-09-27 Python FaceDetector 与 C++ Patrol Service 4.3.1～4.3.3 按学习对话完成实战闭环，Service 记 guided L2；保留独立复现与异常路径验收。见 [今日学习快照](../daily/2026-09-27.md)。

2026-10-05 完成 Chapter 7 / 7.4.1 AMCL 初始位姿发布和 7.4.2 C++ TF 实时位姿查询，实测 x/y/yaw 能随机器人运动连续变化。已进入 7.4.3，查看 `nav2_msgs/action/NavigateToPose` 后开始编写 C++ Action Client，当前停在类型别名、GoalHandle 与 Client 成员定义阶段。实际源码尚在虚拟机，未同步到仓库；详见 [当日学习与踩坑记录](../daily/2026-10-05.md)。

2026-10-06 按当日实际运行完成 7.4.3 `NavigateToPose`、7.4.4 `FollowWaypoints` 与 7.5 巡检控制节点。巡检流程已连接自定义 `SpeechText` Service、speaker + `espeak-ng`、Gazebo camera bridge、`sensor_msgs/msg/Image`、`cv_bridge` 和 OpenCV 到点拍照；解决 `frame_id=msp`、yaw 单位、Timer 重复 Goal、Server 名称、Feedback 字段、rosidl 配置、`GZ_IP` 和 `latest_image_` 判空反向等问题。最终源码尚未从 Ubuntu 同步到仓库，详见 [当日完整记录](../daily/2026-10-06.md)。

2026-10-08 学习至第八章 8.2.3：pluginlib 的 Shape、Square/Triangle、PLUGINLIB_EXPORT_CLASS、plugins.xml、CMake 注册和 ClassLoader；排查大小写、area() const 与 target 依赖问题。8.1.3 插件编译成功由用户明确确认；8.1.4 加载运行未确认。8.2 Planner Server/GlobalPlanner、Path/Twist、五个接口、WeakPtr/SharedPtr、StraightLinePlanner 框架和直线插值均为教学指导，编译及 Gazebo/Nav2 联调未确认。本机未找到源码且 Ubuntu SSH 认证失败，本次只归档文档。见 [当日记录](../daily/2026-10-08.md)。

2026-10-09 已归档直线规划器编译/加载、RViz 与 MPPI 联调确认，Costmap 障碍物拒绝及四/八方向 A*。本次读取真实源码并重新编译运行八方向固定地图，记录循环后显式 unlock 的真实状态，见 [当日记录](../daily/2026-10-09.md)。

2026-10-10 已按学习会话确认 A* 不可达测试、astarSearch() 封装和 start==goal 单点路径，AStarPlanner 插件框架构建与 pluginlib 发现成功。记录 Costmap 转换、Path/MPPI 职责、算法拆分拼写/声明错误及 createPlan() 审查问题；尚无 Gazebo 绕障成功证据，Action 状态与速度超时异常待定位。本次未重跑 ROS2，MASTERY 不变，见 [当日记录](../daily/2026-10-10.md)。

## 完成证据

能够独立建立 package 和节点数据流，解释输入/输出/参数/依赖，使用 ROS 2 命令定位问题，并完成 TF/URDF/RViz 的机器人模型实践。

## 新内容位置

按 workspace 或明确主题建立子目录。不要提交 `build/`、`install/`、`log/`；记录所用发行版与官方文档链接。


## 当前学习顺序

截至 2026-10-10，已推进至 AStarPlanner 集成实现与审查。下一步先确认 waypoint_follower 异常触发阶段、lifecycle 状态和 Planner Server 前序日志，再核验 createPlan() 修正、重新构建并测试实际绕障和异常场景。搜索内部取消、周期重规划、完整 footprint 与切角边界仍待验证。8.1.4 独立加载输出待补，不能由 Nav2 加载结果替代。

第七章 **7.4.3、7.4.4、7.5 已按 10 月 6 日当日运行完成** 的记录继续保留；最终源码同步、干净构建、完整重启、异常路径和独立复现仍待完成。

详见 [10 月 6 日巡检记录](../daily/2026-10-06.md)、[当前断点](../CURRENT.md) 与 [已有源码快照](chapt6_ws/README.md)。10 月 6 日最终源码尚未同步，本次只记录学习会话和运行确认能够证明的进度；独立掌握仍按验收记录，不上调等级。

9 月 29 日的 TF 静态/动态发布与 CLI 查询实测见 [TF 源码快照](tf_test/README.md)，C++ listener 仍为未完成草稿。第四章独立验收及 Turtle Patrol 闭环仍待补。精确断点和证据边界以 [CURRENT.md](../CURRENT.md) 为准。
