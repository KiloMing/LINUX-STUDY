# CURRENT

最后更新：2026-10-12

## 当前进度快照

**第九章最新断点（2026-10-12）：** 已能在 Gazebo 复杂室内场景运行 RTAB-Map RGB-D 建图，但三维地图在平移后依然有重影/扭曲，**未验收通过**。启动脚本明确使用 `/diff_drive_controller/odom`（`visual_odometry=false` / `icp_odometry=false`）；RGB-D PointCloud2、Depth、CameraInfo 的消息 `frame_id` 均为 `camera_optical_frame`，但尚未验证 TF/内参与几何一致。当前 Ubuntu 相机配置请求 120 Hz、640×480，虚拟机实时率约 33%～41%；拟单独测试降为 15 Hz。复杂场景未消除异常，因此下次优先做 **静止单帧点云 → 激光点云对照 → optical TF/CameraInfo → odom 直行叠加 → RTAB-Map 同步/优化**。本次只整理对话和已有仓库信息，未同步 Ubuntu 最终 Xacro、world、launch、mapping.sh，也没有重跑构建。详见 [10 月 12 日 RGB-D 排错记录](daily/2026-10-12.md)。


**第九章最新断点（2026-10-11）：** 学习者确认 Gazebo RGB/Depth 显示、PointCloud2 桥接和 RViz 点云显示已跑通。点云 `frame_id=second_robot/base_footprint/camera_sensor`；临时补充 `camera_link` 到该坐标系的零变换后显示成功。TF 兼容方案仍待工程化，之后继续 RTAB-Map。仓库 camera Xacro 已有 RGB-D 传感器，但没有兼容 TF；旧 launch 仅有 `/clock` bridge，四条相机 bridge 与最终 launch 待从 Ubuntu 同步。本次未重跑仿真，详见 [今日 RGB-D 记录](daily/2026-10-11.md)。

**激光雷达 SLAM + Nav2 仿真已跑通，第八章结束。** 这是学习者在 2026-10-11 明确确认的课程与运行进度；自定义 A* 已进入后续优化阶段。下一阶段主线调整为 **Gazebo RGB-D → RTAB-Map 建图/定位 → Nav2 导航 → Gemini Pro 真机相机 → FET3588 部署 → 实体底盘集成**，以成熟视觉 SLAM 框架的应用和系统集成为目标。

本次只更新文档，没有重跑 ROS2/Gazebo，也没有取得 Ubuntu 中今天的最终 A* 源码和完整日志。课程结束不等于所有异常分支、碰撞安全或独立掌握均已验收；MASTERY 不变。今天的 `planner_server`、`GoalOccupied` 与索引初始化排错主题及证据缺口见 [10 月 11 日记录](daily/2026-10-11.md)，具体下一阶段任务见 [RGB-D 应用路线](ros2/RGBD_ROADMAP.md)。10 月 9、10 日记录保留为当时状态，不再作为当前学习断点。

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

- 第八章课程已结束；待补的是最终源码同步、干净重建与重启证据，以及搜索取消、周期重规划、完整 footprint、非法/被占据起终点和切角边界测试。10 月 10 日 createPlan() 审查项需对最终源码逐项核验。8.1.4 Square/Triangle 独立加载输出仍待补，不阻塞 RGB-D 仿真起步。

- 独立复现 FaceDetector 与 Patrol 的接口、Server、Client，保存最终源码、构建与运行输出；补测非法目标、服务缺席、请求失败、图片读取失败和空检测结果。

- 客户端先检查 `argc`，正确处理 `stoi` 异常和端口范围。
- `send()` 返回值使用 `ssize_t`；`send_all()` / `recv_all()` / 文件写入处理短读写和 `EINTR`。
- 客户端累计接收完整的 2 字节 ACK；服务端检查 `recv_file()` 返回值，子进程执行 `close(client_sock) + _exit()`。
- 空文件、大于 4096 字节文件、包含 `0x00` 的二进制文件逐字节一致；多客户端同时连接，子进程正常回收。
- 明确结构体序列化和 `uint64_t` 字节序，增加最终完成 ACK、校验和和失败文件清理。
- producer-consumer 闭卷独立复现、3P2C 最终运行证据及 rwlock 源码/输出仍待补齐。

## 当前问题/待解决

- A* 路径靠近障碍物：机器人 footprint/碰撞半径、膨胀层与路径代价策略待联合核验；狭窄通道规划和跟踪仍需优化。
- 底盘速度指令时间戳异常与 `Velocity command timed out. Braking.` 尚需定位。检查仿真时钟、消息类型/时间戳、速度发布频率及控制器超时，不能直接断言由 A* 或 waypoint_follower 引起。
- 今天 `planner_server`、`GoalOccupied` 和索引初始化的完整日志/最终改动未返回，暂不把某个具体原因或修复写成已验证结论。
- Gemini Pro 的深度 USB 识别已有引用会话记录（`2bc5:0614`，480 Mbps）；RGB 接口、兼容驱动、RGB-D 对齐、ROS2 数据与 USB 链路稳定性仍待验证。

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

执行 [视觉巡航教学计划第一课](ros2/VISUAL_PATROL_TEACHING_PLAN.md)：先整理相机接口表、当前配置快照和缺失项，再推进以下 RGB-D 传感器验收。教学计划已制定，RGB-D 显示与临时 TF 验证已有上述运行确认，完整课次验收仍待补齐。

下一次主任务：先同步最终运行源码并工程化 TF 兼容方案，再检查 Ubuntu `~/my_ros/chapt6_ws` 的相机模型和 Gazebo/ros_gz 版本，区分普通 RGB 与 RGB-D 传感器；配置 RGB、Depth、CameraInfo 和点云桥接，验证频率、时间戳、深度单位、图像对齐、光学坐标系与 RViz 显示。保持 `/clock` 单一发布，保存配置和短 rosbag。此关通过后再启动 RTAB-Map，详见 [第一阶段验收](ros2/RGBD_ROADMAP.md)。

并行归档：同步今天真正跑通的 A* 源码和日志，补齐 planner_server、GoalOccupied、索引初始化的发生条件与修复前后证据；A* 距障和速度时间戳问题作为遗留项跟进，不再把它们写成“第八章尚未结束”。

第七章待归档与验收清单继续保留：

1. 从 Ubuntu `~/my_ros/chapt6_ws` 同步 10 月 6 日最终运行源码：Action Client、巡检控制器、speaker、`SpeechText.srv`、camera bridge launch、CMake 和 package.xml。
2. 在仓库中先做逐文件 diff，确认 `latest_image_` 已使用 `if (!latest_image_)`，节点名、Server 名、字段和依赖都是最终版本，再归档而不覆盖其他工作。
3. 在干净终端重新构建并重新 source，完整重启 Gazebo、Nav2、speaker 和 patrol controller，排除旧 install 空间造成的假成功。
4. 保存 Action、Service、Topic/QoS、每点到达日志、语音结果和 `photo_A`～`photo_D` 文件证据。
5. 注入 Server/Service/bridge 缺席、Goal aborted/canceled、相机首帧未到和图片路径不可写等错误，确认节点能报告并继续安全运行。
6. 脱离笔记独立复现最小 `FollowWaypoints` Client 和一次完整巡检，再决定是否上调 Action/Service/传感器集成掌握等级。

## 下一步

按 RGB-D 六阶段路线推进，先完成仿真传感器验收，再验证建图、保存/加载、定位和 Nav2。首次视觉导航联调使用官方规划器建立基线，之后再对比自定义 A*。真机相机、FET3588 与底盘分别验收后集成。第七、八章最终源码和独立复现证据继续补齐。

第六章 `ros2_control` 已完成课程收尾：当前 launch 自动启动 joint state 与 diff drive 控制器，并只桥接一次 `/clock`；`scan_bridge` 与 Gazebo Server 统一继承 `gz_env`。仍应在每次完整重启后用控制器列表、publisher 详情、消息与 TF 做运行验收。

9 月 29 日 `ros2/tf_test` 中的 5.3.3 C++ listener 仍是旧的未完成草稿；10 月 5 日课程工作区中的实时位姿查询和 10 月 6 日巡检闭环都已按当日运行完成，但尚未同步最终源码。课程进度、仓库快照与独立掌握三类证据继续分开记录。Turtle Patrol、Service/Parameter/Launch 与历史独立验收继续保留。
