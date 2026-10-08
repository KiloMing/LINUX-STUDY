# CURRENT

最后更新：2026-10-08

## 当前进度快照

**第八章已学习至 8.2.3 直线插值讲解；不等于运行完成。** 8.1 pluginlib 已学习 Shape、Square/Triangle、导出、XML/CMake 注册与 ClassLoader。8.1.3 插件编译成功由用户明确确认；8.1.4 加载测试代码已讲解，实际运行成功尚未确认。8.2 Planner Server/GlobalPlanner、Path/Twist、五个接口、智能指针、StraightLinePlanner 框架和 createPlan 算法均属教学指导，编译及 Gazebo/Nav2 联调未确认。

10 月 8 日归档时，本机未找到第八章源码，Ubuntu SSH 认证失败，未能读取实际 `chapt8_ws`；本次仅更新记录，不生成聊天示例源码，不上调 MASTERY。见 [10 月 8 日学习与排错记录](daily/2026-10-08.md)。以下第七章运行确认及源码待归档事实继续保留。

**第七章历史运行快照：7.4.3、7.4.4 与 7.5 已按 10 月 6 日实际运行完成。** `NavigateToPose` 单点导航和 `/follow_waypoints` 路点导航均已跑通；巡检控制节点能按 A/B/C/D 导航，根据 `current_waypoint` 与最终 Result 识别到点，通过自定义 `SpeechText` Service 调用 speaker + `espeak-ng` 播报，并保存 Gazebo 相机的当前图像。

10 月 6 日已解决 `frame_id=msp` 拼写、yaw 度/弧度混用、Timer 重复发送 Goal、`FollowWaypoints` Server 名称、Feedback 字段、`package.xml`/CMake rosidl 配置、camera bridge 与 `GZ_IP`、以及 `latest_image_` 判空条件写反等问题。完整学习和排错过程见 [10 月 6 日巡检闭环记录](daily/2026-10-06.md)。

当前链路是 `FollowWaypoints Action → 到点事件 → SpeechText Service + Image Subscription`。Gazebo camera 经 `ros_gz_bridge` 转为 `/camera/image` 的 `sensor_msgs/msg/Image`，巡检节点缓存最新帧，到点时经 `cv_bridge` 转为 OpenCV `bgr8` 并用 `cv::imwrite()` 保存。最后路点不能只靠 Feedback 索引变化判断，需要在 `SUCCEEDED` Result 中补处理。

10 月 6 日最终运行结果来自学习者确认。本仓库归档前工作区干净，没有发现从 Ubuntu 虚拟机同步来的当日最终源码；对话附件仍是修复 `latest_image_` 前的中间版本。因此本次只提交记录和索引，不把旧附件当作最终可复现源码，也不声称 Codex/macOS 环境重新完成了 ROS 2 构建。

`collision_monitor` lifecycle 排错规则仍是先查状态再做 transition：`unconfigured` 才 configure，`inactive` 才 activate，已经 `active` 时不重复 configure。Action/Service 字段继续以本机 `ros2 interface show` 为准，不凭其他版本示例猜测。

10 月 3 日完成 `joint_state_broadcaster`、`JointGroupEffortController` 与 `diff_drive_controller`，明确 effort 是旋转关节轴上的力矩目标、velocity 是轮速目标，并区分 command/state interface 与 claimed/unclaimed。`sim_control` / `gz_sim_control` 已整合 Gazebo、`robot_state_publisher`、`/clock` 与 `/scan` bridge、spawn 以及 joint state/diff drive 控制器自动激活。

已修复两条关键链路：`/clock` 重复 publisher 会使 `slam_toolbox` 报 `Detected jump back in time`，因此只保留一次时钟桥接；`scan_bridge` 必须使用 `additional_env=gz_env`，否则其 `GZ_IP` 与 Gazebo Server 不一致，Gazebo `/scan` 无法桥接到 ROS2。Gazebo 话题列举命令为 `gz topic -l`。

已安装并启动 `slam_toolbox`，理解 `map→odom→base_footprint→base_link→laser_link`、`/map`、`/scan`、OccupancyGrid 和 RViz `Fixed Frame=map`。10 月 4 日直线测试中 Gazebo 位移约 5.65768 m、odom 约 5.660 m，基本一致，轮径和轮距暂不再改。见 [10 月 5 日定位与导航记录](daily/2026-10-05.md)、[10 月 4 日建图记录](daily/2026-10-04.md) 与 [已有工程快照](ros2/chapt6_ws/README.md)。

10 月 5 日的定位与 TF 结果依据学习者自述、学习会话与截图归档；当时本仓库未同步虚拟机中的 `local_init.cpp`、TF listener 或 `action_client.cpp`。10 月 6 日仍延续相同证据边界：课程运行进度、仓库可复现源码和独立验收分别记录，不因 guided integration 跑通而直接上调 MASTERY。

## 仓库已验证（2026-09-29 TF 快照）

- 实际工程来自虚拟机 ~/my_ros/tf_test，静态 (5,3,0)/60°，动态 (2,3,2)/30°，动态 sendTransform 已存在。
- 两个发布器重新构建成功；tf2_echo 三组查询成功，包括 base_link → target_point = (2.598,-1.500,-2.000)/30°。
- CMake/package.xml 无重复 tf2_geometry_msgs；两个目标已配置依赖和安装。
- tf_listen 仍是未完成草稿，保存为 .incomplete，不参与编译；不能把 CLI 成功写成 C++ 查询节点完成。
- 源码快照不含 build/install/log；实际工作区 src 下本次未发现这些误生成目录。

## 待验证

- 第八章：实际 chapt8_ws 源码同步、8.1.4 插件加载输出、8.2 框架编译、直线路径边界处理及 Gazebo/Nav2 联调。8.1.3 用户确认编译成功不代表这些项目已完成。

- 独立复现 FaceDetector 与 Patrol 的接口、Server、Client，保存最终源码、构建与运行输出；补测非法目标、服务缺席、请求失败、图片读取失败和空检测结果。

- 客户端先检查 `argc`，正确处理 `stoi` 异常和端口范围。
- `send()` 返回值使用 `ssize_t`；`send_all()` / `recv_all()` / 文件写入处理短读写和 `EINTR`。
- 客户端累计接收完整的 2 字节 ACK；服务端检查 `recv_file()` 返回值，子进程执行 `close(client_sock) + _exit()`。
- 空文件、大于 4096 字节文件、包含 `0x00` 的二进制文件逐字节一致；多客户端同时连接，子进程正常回收。
- 明确结构体序列化和 `uint64_t` 字节序，增加最终完成 ACK、校验和和失败文件清理。
- producer-consumer 闭卷独立复现、3P2C 最终运行证据及 rwlock 源码/输出仍待补齐。

## 当前问题/待解决

- 10 月 6 日的最终 `NavigateToPose`、`FollowWaypoints`、巡检控制器、speaker、`SpeechText.srv` 与 camera bridge 源码仍在 Ubuntu 实际工作区，尚未同步到本仓库。下一次必须从最终运行版本同步，而不是使用对话附件中的修复前快照。
- 仓库内尚不能重新检查当天 CMake/package.xml 的 Action、rosidl、`sensor_msgs`、`cv_bridge` 与 OpenCV 配置，也没有保存干净构建、完整重启和 A/B/C/D 每点一次语音/照片的运行证据。
- 多圈巡检需要进一步验证点位索引重置、每点只触发一次、最后点补触发和照片覆盖策略；保存文件名应加入圈数或时间戳。
- 仍需测试 Action Server、Speech Service、camera bridge 缺席，以及 Goal aborted/canceled、首帧未到、图片路径不可写和图像转换失败等异常路径。
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

第八章优先：恢复 Ubuntu 访问并定位实际 `chapt8_ws`；核对并同步真实源码，重新构建插件、运行 8.1.4 加载测试并保留输出。随后核对本机 GlobalPlanner 接口、构建 StraightLinePlanner、检查直线插值边界，再做 Gazebo/Nav2 联调。详见 [10 月 8 日测试清单](daily/2026-10-08.md)。

第七章待归档与验收清单继续保留：

1. 从 Ubuntu `~/my_ros/chapt6_ws` 同步 10 月 6 日最终运行源码：Action Client、巡检控制器、speaker、`SpeechText.srv`、camera bridge launch、CMake 和 package.xml。
2. 在仓库中先做逐文件 diff，确认 `latest_image_` 已使用 `if (!latest_image_)`，节点名、Server 名、字段和依赖都是最终版本，再归档而不覆盖其他工作。
3. 在干净终端重新构建并重新 source，完整重启 Gazebo、Nav2、speaker 和 patrol controller，排除旧 install 空间造成的假成功。
4. 保存 Action、Service、Topic/QoS、每点到达日志、语音结果和 `photo_A`～`photo_D` 文件证据。
5. 注入 Server/Service/bridge 缺席、Goal aborted/canceled、相机首帧未到和图片路径不可写等错误，确认节点能报告并继续安全运行。
6. 脱离笔记独立复现最小 `FollowWaypoints` Client 和一次完整巡检，再决定是否上调 Action/Service/传感器集成掌握等级。

## 下一步

课程讲解已推进到第八章 8.2.3，先补第八章源码与运行证据。第七章 7.5 巡检闭环的当日运行确认保持有效，仍需同步 Ubuntu 中真正跑通的最终源码，并做干净重建、完整重启、异常路径与独立复现验收。

第六章 `ros2_control` 已完成课程收尾：当前 launch 自动启动 joint state 与 diff drive 控制器，并只桥接一次 `/clock`；`scan_bridge` 与 Gazebo Server 统一继承 `gz_env`。仍应在每次完整重启后用控制器列表、publisher 详情、消息与 TF 做运行验收。

9 月 29 日 `ros2/tf_test` 中的 5.3.3 C++ listener 仍是旧的未完成草稿；10 月 5 日课程工作区中的实时位姿查询和 10 月 6 日巡检闭环都已按当日运行完成，但尚未同步最终源码。课程进度、仓库快照与独立掌握三类证据继续分开记录。Turtle Patrol、Service/Parameter/Launch 与历史独立验收继续保留。
