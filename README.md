# LINUX-STUDY

从 Linux/C++ 系统能力出发，逐步走向嵌入式、ROS 2 与移动机器人系统实践的长期学习仓库。

主线不是单独押注 ROS 或 SLAM，而是形成可迁移的 Linux/C++、通信、调试和嵌入式能力，再用于底盘、传感器、定位与导航：

```text
Linux/C++ 系统能力
  → Socket/Linux 调试
  → CMake/C++ 工程化
  → ROS2
  → TF2/URDF/RViz/仿真
  → 运动学/Odometry
  → STM32+ROS2 底盘/ros2_control
  → LiDAR/IMU/状态估计
  → SLAM/Nav2
  → 综合机器人项目
```

这个仓库不只保存笔记，也保存学习路线、当前检查点、掌握证据和能够运行的工程。聊天内容可能变长或丢失上下文，因此学习进度以仓库记录为准。

## 当前事实

- 仓库保留 DAY1–DAY6 原始记录，daily 学习快照已更新至 2026-10-06。
- 学习主题已从 Linux 基础、Git/Makefile、文件 I/O、进程、IPC、semaphore、mutex，推进到 ROS2 通信、TF、URDF 与 Xacro；各主题的独立掌握程度以验收证据为准。
- 2026-09-11，2 Producer + 2 Consumer 已实际编译运行并正常退出；由于参考过完整答案，producer-consumer 保留 L2，不升 L3。
- 当前已进入 TCP/Socket 基础实践：Echo Server 与教学引导下的多进程文本文件传输已跑通，Socket 当前记录为 L2。
- condition variable、rwlock 及后续阶段均须通过独立任务确认，不直接写成“已掌握”。
- 原始笔记与源码保留原路径；发现的程序问题作为后续调试练习，不在整理时偷偷修正。

## 最新学习进度：2026-10-06

**Chapter 7 / 7.4.3、7.4.4 与 7.5 已按当日运行完成。** `NavigateToPose` 单点导航和 `FollowWaypoints` 路点导航均已跑通，并组合出 A/B/C/D 巡检流程；到点后通过自定义 `SpeechText` Service 调用 speaker + `espeak-ng` 播报，同时保存 Gazebo 相机图像。

今天完整排查了 `frame_id=msp`、yaw 单位、Timer 重复发 Goal、Action Server 名称、Feedback 字段、rosidl 配置、camera bridge/`GZ_IP` 和 `latest_image_` 判断反向等问题。10 月 6 日最终源码仍在 Ubuntu 实际工作区，尚未同步到本仓库；当前完成状态来自当日真实运行确认，不冒充仓库内重建证据。

见 [今日详细学习与踩坑记录](daily/2026-10-06.md)、[CURRENT.md](CURRENT.md) 和 [ROS 2 主题索引](ros2/README.md)。

## 学习节奏

- 普通周投入 8–12 小时；考试周可降到约 5 小时，新内容顺延，验收标准不降。
- 每约 12 周按 70% 通用核心、20% 机器人、10% 前沿探索检查投入，并做一次职业/行业校准。
- 时间只用于安排投入；能解释、独立复现、完成变式并留下调试证据，才进入下一阶段。

完整阶段时间、任务和验收标准见 [ROADMAP.md](ROADMAP.md)。

## 三层学习记忆

1. [ROADMAP.md](ROADMAP.md)：保存稳定的长期路线和阶段门槛。
2. [CURRENT.md](CURRENT.md)：保存当前真实状态、薄弱点和下一次测试。
3. [daily/](daily/) 与主题代码：保存每天的过程、错误、实验和可复现证据。

发生冲突时，优先相信可运行代码和测试，其次是近期 `CURRENT.md`，再其次才是聊天中的口头描述。

## 从这里开始

- [当前进度](CURRENT.md)
- [长期路线](ROADMAP.md)
- [掌握度标准](MASTERY.md)
- [实际环境](ENVIRONMENT.md)
- [教学协议](TEACHING_PROTOCOL.md)

## 历史学习记录

- [DAY1](DAY1/README.md)：Linux 环境、权限、文件操作、Vim、重定向、grep、编译与 Git 基础。
- [DAY2](DAY2/README.md)：Git branch 与 Makefile 入门，含多文件 C++ Makefile 练习。
- [DAY3](DAY3/README.md)：Linux 文件管理、文件 I/O、目录遍历与 pipe 练习。
- [DAY4](DAY4/README.md)：进程、`fork/exec/wait` 与 Mini Shell。
- [DAY5](DAY5/20260827.md)：FIFO、`mmap` 与进程间通信。
- [DAY6](DAY6/readme.md)：`mmap`、semaphore 与 mutex 练习。

## 近期每日记录

- [2026-10-06](daily/2026-10-06.md)：完成 NavigateToPose、FollowWaypoints 与巡检控制节点；串联 SpeechText/espeak-ng、Gazebo camera bridge、cv_bridge/OpenCV 拍照，并详录 frame、yaw、Timer、rosidl、GZ_IP 和判空错误。
- [2026-10-05](daily/2026-10-05.md)：AMCL 初始位姿、四元数/yaw、TF 实时位姿、NavigateToPose Action 与 C++ Client 起步，以及 lifecycle/类型/语法踩坑。
- [2026-10-04](daily/2026-10-04.md)：SLAM 重影排查、静态仓储场景、20Hz LiDAR、弧线建图与地图保存成功截图。
- [2026-10-03](daily/2026-10-03.md)：ros2_control 收尾、时钟与扫描桥接修复，进入 7.2.1 在线建图。
- [2026-10-02](daily/2026-10-02.md)：Gazebo 传感器、GZ_IP、ros2_control 6.5.1/6.5.2 与完整排错、源码和日志快照。

- [2026-10-01](daily/2026-10-01.md)：Xacro 部件总装、碰撞与惯性层级排错、RViz/TF 截图及实际源码验证。
- [2026-09-30](daily/2026-09-30.md)：URDF/RViz 模型显示、黑屏排查解决、Xacro 参数与 macro，以及 6.2.4 模块化续学位置。
- [2026-09-29](daily/2026-09-29.md)：C++ 静态/动态 TF 实测、tf2_echo 查询与 C++ listener 草稿。
- [2026-09-28](daily/2026-09-28.md)：依据实际工程核对 Turtle Patrol 进度与待验证项。

- [2026-09-27](daily/2026-09-27.md)：Python 人脸检测与 C++ Patrol Service 闭环、环境/构建排错及 4.4.1 续学快照。

- [2026-09-26](daily/2026-09-26.md)：Topic 闭环、SystemStatus 接口、Python 发布、Qt 显示与 Service 续学快照。
- [2026-09-25](daily/2026-09-25.md)：turtlesim Twist 发布画圆、Pose 订阅、CMake 多 executable 与未验证闭环控制快照。

- [2026-09-24](daily/2026-09-24.md)：ROS2 Topic 实际收发、C++ espeak-ng 语音输出、链接排错与 callback/worker thread 解耦。
- [2026-09-23](daily/2026-09-23.md)：Timer 定时发布 time_topic、Subscription 与 Executor 基础、CMake target 错误。

- [2026-09-22](daily/2026-09-22.md)：ROS2 Node/Topic、Lambda/callback、cpp-httplib 与 CMake include path，保留待验证问题。
- [2026-09-21](daily/2026-09-21.md)：CMake target、依赖与 ROS2 package 构建链。
- [2026-09-20](daily/2026-09-20.md)：poll 多客户端与 epoll LT 入门。

- [2026-09-19](daily/2026-09-19.md)：Select/bitmap/fd_set、监听 fd 与连接 fd、最小多客户端 Echo Server 学习版。
- [2026-09-18](daily/2026-09-18.md)：多进程 TCP 文件传输、元信息、ACK、二进制字节流与调试记录。
- [2026-09-15](daily/2026-09-15.md)：最小 Echo Server、TCP 合并读取、阻塞行为与当前源码核对。
- [2026-09-13](daily/2026-09-13.md)：Socket 起步、IPv4 地址结构、两个 fd 与资源生命周期。
- [全部每日记录](daily/README.md)

## 后续主题

- [Linux 系统编程](linux/)
- [Socket](socket/)
- [CMake](cmake/)
- [ROS 2](ros2/)
- [SLAM 与导航](slam/)
- [综合项目](projects/)
- [每日记录](daily/)

## 每次学习流程

```text
读取 CURRENT.md
    ↓
不看笔记完成“下一次测试”
    ↓
根据结果回补或学习新内容
    ↓
独立复现 → 变式 → 制造/排查错误 → 实际应用
    ↓
更新 daily、CURRENT 和 MASTERY
    ↓
提交能够说明真实进展的代码与记录
```

仓库结构和状态检查：

```bash
python3 scripts/validate_repository.py
```
