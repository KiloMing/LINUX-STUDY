# CURRENT

最后更新：2026-10-05

## 当前进度快照

**学习快照/当前课程进度：7.4.1 与 7.4.2 已完成，7.4.3 已进入。** 已完成 AMCL 初始位姿发布相关练习，并通过 C++ TF listener 周期查询机器人在 `map` 中的实时 x/y/yaw；静止时输出稳定，移动时位置与 yaw 连续变化。当前正在编写 Nav2 `NavigateToPose` C++ Action Client，停在 Action 类型别名、GoalHandle 与 Client 成员定义阶段。

7.4.3 尚未完成：`action_client.cpp` 还需补构造函数、`create_client()`、`send_goal()`、goal response/feedback/result callbacks、`CMakeLists.txt` 与 `package.xml`，之后才能编译并进行实际单点导航验证。今天已定位 `NavigateToPose` 类型别名缺失和 class 结尾缺少分号两个错误；不能把“编辑器红线已解释”写成“客户端已编译通过”。

`collision_monitor` lifecycle 排错的当前规则是先查状态再做 transition：`unconfigured` 才 configure，`inactive` 才 activate，已经 `active` 时不重复 configure。`NavigateToPose` 字段以本机 `ros2 interface show nav2_msgs/action/NavigateToPose` 为准，今天截图确认 Result 包含 `uint16 error_code` 与 `string error_msg`。

10 月 3 日完成 `joint_state_broadcaster`、`JointGroupEffortController` 与 `diff_drive_controller`，明确 effort 是旋转关节轴上的力矩目标、velocity 是轮速目标，并区分 command/state interface 与 claimed/unclaimed。`sim_control` / `gz_sim_control` 已整合 Gazebo、`robot_state_publisher`、`/clock` 与 `/scan` bridge、spawn 以及 joint state/diff drive 控制器自动激活。

已修复两条关键链路：`/clock` 重复 publisher 会使 `slam_toolbox` 报 `Detected jump back in time`，因此只保留一次时钟桥接；`scan_bridge` 必须使用 `additional_env=gz_env`，否则其 `GZ_IP` 与 Gazebo Server 不一致，Gazebo `/scan` 无法桥接到 ROS2。Gazebo 话题列举命令为 `gz topic -l`。

已安装并启动 `slam_toolbox`，理解 `map→odom→base_footprint→base_link→laser_link`、`/map`、`/scan`、OccupancyGrid 和 RViz `Fixed Frame=map`。10 月 4 日直线测试中 Gazebo 位移约 5.65768 m、odom 约 5.660 m，基本一致，轮径和轮距暂不再改。见 [10 月 5 日定位与导航记录](daily/2026-10-05.md)、[10 月 4 日建图记录](daily/2026-10-04.md) 与 [已有工程快照](ros2/chapt6_ws/README.md)。

今日运行结果依据学习者自述、学习会话与截图归档。本仓库未同步 10 月 5 日虚拟机中的 `local_init.cpp`、TF listener 或 `action_client.cpp`，也没有重新构建 Nav2 客户端；课程完成状态与仓库可复现证据、独立验收分别记录，不上调 MASTERY。

## 仓库已验证（2026-09-29 TF 快照）

- 实际工程来自虚拟机 ~/my_ros/tf_test，静态 (5,3,0)/60°，动态 (2,3,2)/30°，动态 sendTransform 已存在。
- 两个发布器重新构建成功；tf2_echo 三组查询成功，包括 base_link → target_point = (2.598,-1.500,-2.000)/30°。
- CMake/package.xml 无重复 tf2_geometry_msgs；两个目标已配置依赖和安装。
- tf_listen 仍是未完成草稿，保存为 .incomplete，不参与编译；不能把 CLI 成功写成 C++ 查询节点完成。
- 源码快照不含 build/install/log；实际工作区 src 下本次未发现这些误生成目录。

## 待验证

- 独立复现 FaceDetector 与 Patrol 的接口、Server、Client，保存最终源码、构建与运行输出；补测非法目标、服务缺席、请求失败、图片读取失败和空检测结果。

- 客户端先检查 `argc`，正确处理 `stoi` 异常和端口范围。
- `send()` 返回值使用 `ssize_t`；`send_all()` / `recv_all()` / 文件写入处理短读写和 `EINTR`。
- 客户端累计接收完整的 2 字节 ACK；服务端检查 `recv_file()` 返回值，子进程执行 `close(client_sock) + _exit()`。
- 空文件、大于 4096 字节文件、包含 `0x00` 的二进制文件逐字节一致；多客户端同时连接，子进程正常回收。
- 明确结构体序列化和 `uint64_t` 字节序，增加最终完成 ACK、校验和和失败文件清理。
- producer-consumer 闭卷独立复现、3P2C 最终运行证据及 rwlock 源码/输出仍待补齐。

## 当前问题/待解决

- 7.4.3 `NavigateToPose` C++ Action Client 只完成到类型定义阶段。虚拟机源码需要补 `using NavigateToPose = nav2_msgs::action::NavigateToPose;`、GoalHandle、class 结尾分号和后续完整实现，再做构建与运行验证。
- 10 月 5 日代码尚未同步到仓库，当前无法在仓库内检查 CMake/package.xml 或重现编译错误；后续要归档源码和最小构建/运行证据。
- `collision_monitor` lifecycle 操作必须先查状态，避免把“已 inactive/active 时重复 configure 被拒绝”误判为节点故障。
- SLAM 重影问题已在今天的建图操作中明显改善并完成地图保存。当前仿真仍对原地旋转和碰撞敏感，使用 `u/o/m` 等弧线运动，避免 `j/l` 原地旋转及撞墙；这不是对所有差速机器人的普遍限制。
- 直线 odom 与 Gazebo 基本一致，轮径/轮距暂不再改。昨天提出的轮子惯量方向检查尚无完成证据，不再作为推进地图加载的前置阻塞项。
- 后续补录 `/scan` 实际频率与重启复现证据，同步最新 20Hz LiDAR 配置、`testworld.sdf`、匹配 world 名的 launch 和地图原文件。

- 9 月 25 日旧草稿遗留核验（9 月 26 日已学习阈值和限幅，最终源码尚未核验）：将 `cmd_vel.linear.y = k_ang * error_ang` 修正为 `angular.z`；确认 `<cmath>`/`M_PI`；核对闭环 executable 的 CMake 依赖与安装。`target_ang_` 可局部化；旧草稿的 `max_speed_` 未实际限幅，今日对话代码已出现限幅与 angular.z，但最终工程仍需归档核验。

- A) cpp-httplib 头文件引用路径尚需在实际工程中验证。对话中的目录是 `include/cpp-httplib/httplib.h`；视频的 `include_directories(include)` 配套 `#include "cpp-httplib/httplib.h"`，当前 `<httplib.h>` 则要求搜索起点指向 `include/cpp-httplib`。
- B) 需要继续理解 `start_download` 的线程创建与 callback 生命周期；实际源码未核验，不能直接认定异步方式或引用安全。
- C) Timer Publisher → Topic → Subscription 已实际跑通；仍需用 `ros2 topic info/echo/hz` 保存 CLI 观察证据，并从空白独立复现一次。
- D) espeak-ng 已接入 Subscription 并完成语音输出；已解决普通 library 链接与 plain/keyword signature 冲突。当前 queue + `speech_thread` 解耦代码尚待实际运行验证。
- E) TF 学习环境已确认 Ubuntu 24.04.5 / aarch64 / Jazzy，见 [ENVIRONMENT.md](ENVIRONMENT.md)。

## 下一次测试

1. 补全 `NavigateToPoseClient` 构造函数并用 `create_client<NavigateToPose>(this, "/navigate_to_pose")` 创建客户端。
2. 实现 `send_goal()`，在 `map` frame 中设置自由区域目标 x/y/yaw，并把 yaw 转成 quaternion。
3. 完成 goal response、feedback、result 三个回调，区分“目标被接受”与“最终导航成功”。
4. 更新实际工程的 `CMakeLists.txt` 与 `package.xml`，完成 `colcon build`、source、executable 查询和运行验证。
5. 启动 Nav2 后先检查 `/navigate_to_pose` server 与 `collision_monitor` lifecycle 状态，再发送目标；保存 accepted、feedback、result 和机器人到达证据。
6. 将 10 月 5 日实际源码及构建证据同步到仓库；在此之前不把 7.4.3 标记完成。

## 下一步

从 7.4.3 Action Client 的构造函数继续：创建 `/navigate_to_pose` client，补 `send_goal()` 与三个回调，再更新构建依赖并进行单点导航实测。7.4.1/7.4.2 已完成，不重复回退；7.4.3 只有在编译通过、Nav2 接受目标、持续收到反馈并最终成功到达后才完成。

第六章 `ros2_control` 已完成课程收尾：当前 launch 自动启动 joint state 与 diff drive 控制器，并只桥接一次 `/clock`；`scan_bridge` 与 Gazebo Server 统一继承 `gz_env`。仍应在每次完整重启后用控制器列表、publisher 详情、消息与 TF 做运行验收。

9 月 29 日 `ros2/tf_test` 中的 5.3.3 C++ listener 仍是旧的未完成草稿；10 月 5 日课程工作区中的实时位姿查询已按截图跑通，但尚未同步到仓库。两份证据不混写：课程进度记 7.4.2 完成，仓库旧草稿仍保持原状态。Turtle Patrol、Service/Parameter/Launch 与历史独立验收继续保留。
