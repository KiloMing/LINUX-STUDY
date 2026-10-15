# LINUX-STUDY

从 Linux/C/C++ 系统编程出发，经过并发、网络通信和工程构建，逐步进入 ROS 2、差速机器人仿真、激光 SLAM、Nav2、自定义 A* 与 RGB-D 视觉 SLAM 的个人学习档案。

**本 README 是本轮学习归档的收尾总结，整理于 2026-10-15；学习记录截至 2026-10-14。** 内容依据默认分支 `main` 的文件、34 篇日报和截至 `256e27b` 的 63 条提交整理。本次收尾只更新根目录 README，不修改历史实验源码、不替未完成项目补造运行结果，也不代表 GitHub 仓库被设置为只读归档。

## 先看结论

- **已留下源码：** DAY2–DAY6 的构建、文件、进程、IPC 与同步练习；2026-09-29 的 TF C++ 发布器；2026-10-02 的机器人模型、Gazebo 和 ros2_control 接入快照。
- **有历史运行记录：** TCP Echo 与文本文件传输、ROS 2 Topic/Service、TF 查询、激光建图及地图保存、Nav2 单点/路点导航、到点播报与拍照。部分结果来自学习者确认，最终源码不全在本仓库。
- **课程已推进但验收未全部完成：** 10 月 11 日确认激光 SLAM + Nav2 仿真跑通、第八章结束；自定义 A* 的距障、碰撞边界、取消和重规划仍需补证。
- **当前未解决：** RTAB-Map RGB-D 地图重影、视觉里程计运动后失跟/漂移、动态 TF 间歇中断。10 月 14 日的 `/camera_odom` 实验尚未验收通过，不能作为稳定视觉导航成果。
- **后续计划：** RGB-D 建图/定位与 Nav2 联调、Gemini Pro 真机相机、FET3588 部署及实体底盘集成。尚无这些阶段完整通过的证据。

这里的“做过实验”“课程跑通”“源码已归档”“能独立复现”分别记录。仓库不是一个从根目录一键构建即可运行的完整机器人产品。

## 阅读入口与证据规则

| 入口 | 内容 |
|---|---|
| [CURRENT.md](CURRENT.md) | 当前断点、已知问题、下一次测试；最新为 10 月 14 日 |
| [ROADMAP.md](ROADMAP.md) | 长期阶段、前置知识和验收门槛 |
| [MASTERY.md](MASTERY.md) | L0–L5 掌握度与独立复现证据 |
| [ENVIRONMENT.md](ENVIRONMENT.md) | 历史已确认环境及待核实项 |
| [daily/](daily/) | 2026-09-07 至 2026-10-14 的 34 篇学习记录，日期不连续 |
| [TEACHING_PROTOCOL.md](TEACHING_PROTOCOL.md) | 回顾、提示、独立实现、变式与延迟复测流程 |
| [RGB-D 路线](ros2/RGBD_ROADMAP.md) / [视觉巡航计划](ros2/VISUAL_PATROL_TEACHING_PLAN.md) | 六阶段迁移路线及第九至十五章、24 个学习单元 |

阅读时必须区分三类依据：**仓库实际源码/输出**、**日报中的历史实测或学习者反馈**、**尚未执行的建议与计划**。遇到冲突，核对文件日期、实际源码和对应实验记录。根目录旧 README 曾停在 10 月 11 日，部分主题索引和 `MASTERY.md` 主表也保留更早状态；不能把旧断点当成最新课程进度，也不能因新课程跑通就自动上调掌握度。

## 文件结构与源码边界

```text
LINUX-STUDY/
├── README.md                         # 本次收尾总览
├── CURRENT.md / ROADMAP.md / MASTERY.md
├── ENVIRONMENT.md / TEACHING_PROTOCOL.md
├── DAY1/                             # Linux、Vim、Git 笔记
├── DAY2/Project_demo/                 # Makefile、多目录 C++ 源码和头文件
├── DAY3/file_demo/                    # 文件 I/O、递归目录遍历、pipe
├── DAY4/                             # fork/exec/wait、单次命令 Mini Shell
├── DAY5/                             # FIFO、文件 mmap、共享映射读写
├── DAY6/                             # mmap + semaphore / mutex
├── daily/                            # 日期记录、模板、历史索引
│   └── assets/                       # RViz、地图保存与视觉失跟截图
├── linux/ / socket/ / cmake/          # 主题索引、证据及验收任务
├── ros2/
│   ├── README.md
│   ├── 2026-09-25-cycle-contarl-UNVERIFIED.md
│   ├── RGBD_ROADMAP.md / VISUAL_PATROL_TEACHING_PLAN.md
│   ├── tf_test/
│   │   ├── src/demo_cpp_tf/           # 静态、动态 TF 发布器
│   │   └── drafts/                   # 不参与编译的 listener 草稿
│   └── chapt6_ws/
│       ├── src/mybot_description/     # Xacro、launch、config、worlds
│       └── evidence/2026-10-02/       # 节点、接口、话题和传感器样本摘要
├── slam/ / projects/                 # 导航/综合项目索引，非完整实现工程
├── docs/superpowers/                 # 9 月 6 日仓库重组设计与实施计划
├── scripts/validate_repository.py    # 仓库结构、链接和生成物检查
├── tests/test_validate_repository.py # 检查器测试
└── .gitignore                        # 排除主要构建产物
```

`branch_test.txt`、`DAY4/ls` 是历史空文件，不是功能入口；DAY2 的 `build/readme.md` 是目录说明，不是已构建结果。

**源码不全位于本仓库：** 后续并发和 Socket 练习主要保存在独立仓库 [LinuxCodeSrc](https://github.com/KiloMing/LinuxCodeSrc)。本仓库 Socket 索引记录了 [TCP 文件传输提交](https://github.com/KiloMing/LinuxCodeSrc/commit/bebcefb7c156a968e79fcfc4153c12d1cee39524) 和 [select Echo 提交](https://github.com/KiloMing/LinuxCodeSrc/commit/e4da15c4df4fea2fc0d4bc736973c1ae7b17f1ae)；这些是历史引用，本次收尾没有重新验收外部仓库。

ROS 2 的第七、八章最终运行工程、巡检节点、自定义 A* 插件、地图原文件及最新 RGB-D 脚本主要留在 Ubuntu 工作区，**未完整同步到这里**。日报中的命令和代码片段不能替代这些缺失文件。

## 学习路线与各阶段成果

```text
Linux 命令 / Git / C/C++
  → Makefile / 文件 I/O / 进程 / IPC
  → pthread / 条件变量 / 生产者消费者 / 读写锁
  → TCP Socket / 文件传输 / select / poll / epoll LT
  → CMake / C++ 工程化
  → ROS 2 Topic / Service / Parameter / Launch
  → TF2 / URDF / Xacro / RViz / Gazebo
  → ros2_control / 差速里程计 / 激光 SLAM
  → AMCL / Nav2 Action / 巡检 / pluginlib / A*
  → RGB-D / RTAB-Map / 视觉里程计（当前排错）
  → 视觉导航 / 真机相机 / FET3588 / 实体底盘（后续）
```

### 1. Linux、Git 与多文件构建：8 月 21–24 日

[DAY1](DAY1/README.md) 记录 Linux 环境、文件与权限、Vim、重定向、grep、编译和 Git；[DAY2](DAY2/README.md) 进入分支与 Makefile。`Project_demo` 用三个子目录源文件、公共头文件和一个 main 练习 C++17 编译、链接、对象文件及 `.d` 头文件依赖。

源码存在，但 Makefile 不会自动创建 `build/`、`bin/`；对象文件按文件名扁平化，不同目录的同名 `.cpp` 可能冲突。它是构建入门练习，独立工程设计与分支冲突处理仍需复测。

### 2. 文件、进程和 IPC：8 月 25–30 日

- [DAY3](DAY3/README.md)：`open/read/write/close`、复制文件、`opendir/readdir/stat`、递归遍历和匿名管道。
- [DAY4](DAY4/README.md)：`fork`、`exec`、`wait/waitpid`；Mini Shell 将一行输入分词后执行一次命令，并非完整交互式 Shell。
- [DAY5](DAY5/20260827.md)：命名管道 FIFO、文件映射 `mmap`、共享映射读写及资源生命周期。
- [DAY6](DAY6/readme.md)：匿名共享映射中的计数器、进程间 semaphore 与 mutex 尝试。

这些原始代码保留学习时状态。管道 EOF、错误处理、映射长度、同步初始化等问题见下方“已知问题”，不能仅以能编译认定正确。

### 3. pthread 与并发：9 月 7–13 日

从单槽生产者—消费者推进到容量 5 的 `std::queue`、多生产者/消费者；学习 mutex 保护共享条件、`pthread_cond_wait` 的释放/重新加锁、`while` 重检、signal/broadcast、生产完成计数和安全退出。

[9 月 11 日](daily/2026-09-11.md) 的 2P2C 有编译运行并正常退出的学习者确认，但参考过完整答案，保持 L2。[9 月 12 日](daily/2026-09-12.md) 的 3P2C 完成改写讨论，缺最终运行证据。[9 月 13 日](daily/2026-09-13.md) 的 2R2W 读写锁有独立运行反馈，源码/输出和延迟复测仍待补存。不能把这些合并成“并发章节已独立掌握”。

### 4. TCP 与 I/O 多路复用：9 月 13–20 日

[Socket 索引](socket/README.md) 汇总 IPv4、网络字节序、监听/连接 fd、阻塞、TCP 字节流和资源清理。已记录最小 Echo、TCP 多次发送被合并读取，以及 `file_Info → OK → 文件正文` 的多进程文本传输。

随后学习 select 的 `fd_set/maxfd`、poll 的 `events/revents` 与连接槽位、epoll LT 的注册/等待/删除。Socket 保持 L2；poll 独立多客户端输出、epoll LT 闭卷实现仍缺证据。非阻塞 ET、UDP、显式序列化、最终 ACK/校验及机器人遥测项目是后续任务，不是已实现功能。

### 5. CMake、C++ 与 ROS 2 通信：9 月 21–28 日

[9 月 21 日](daily/2026-09-21.md) 起学习 target、库/可执行文件、include/link 依赖、out-of-source build、ament/colcon；结合 ROS 2 练习 Lambda、callback、智能指针、对象生命周期和异步 future。

- [9 月 22–24 日](daily/2026-09-24.md)：Timer Publisher → Topic → Subscription 实际收发，并接入 `espeak-ng` 语音；普通库链接和 CMake signature 冲突有排错记录。queue + worker thread 解耦方案仍待最终运行核验。
- [9 月 25–26 日](daily/2026-09-26.md)：turtlesim 速度发布、Pose 订阅、阈值/限幅；自定义 SystemStatus、Python 发布和 Qt 显示。9 月 25 日闭环草稿明确标为 UNVERIFIED。
- [9 月 27 日](daily/2026-09-27.md)：Python FaceDetector 与 C++ Patrol 的自定义 Service、Server/Client 闭环有教学实战记录。
- [9 月 28 日](daily/2026-09-28.md)：Parameter、Launch、namespace/remapping 已做引导练习；独立 Turtle Patrol 工程当时仅到 Service + 目标状态 + Pose 订阅打印，不能与后来第七章巡检闭环混为一谈。

### 6. TF、建模与仿真控制：9 月 29 日–10 月 3 日

[TF 快照](ros2/tf_test/README.md) 保存两个 C++ 发布器：静态 `map→target_point` 为 `(5,3,0)` / yaw 60°；动态 `map→base_link` 每 10 ms 发布 `(2,3,2)` / yaw 30°，**动态发布机制不代表位姿数值在运动**。9 月 29 日有构建及三组 `tf2_echo` 查询记录，C++ listener 仍是 `.incomplete` 草稿。

[模型与控制快照](ros2/chapt6_ws/README.md) 包含 base、双轮、脚轮、LiDAR、IMU、RGB-D camera，学习 Xacro 宏、link/joint、visual/collision/inertial、TF 和 Gazebo 插件。历史 Xacro 检查得到 10 links / 9 joints；10 月 2 日保存节点、硬件接口和传感器证据。

10 月 3 日记录完成 joint_state、effort 与 diff_drive 控制器课程，修复重复 `/clock` 导致的时间回退，以及 scan bridge 未继承同一 `GZ_IP` 的通信问题。**这些后续修改未完整写回当前 10 月 2 日源码快照。**

### 7. 激光 SLAM、定位、导航与巡检：10 月 4–6 日

- [10 月 4 日](daily/2026-10-04.md)：slam_toolbox 在静态仓储场景建图，20 Hz LiDAR、低速弧线运动后效果改善，保存 `test_map.pgm` / `test_map.yaml`，有[保存截图](daily/assets/2026-10-04/slam-map-saved.png)。一次直线比较为 Gazebo 约 5.65768 m、odom 约 5.660 m；这不是完整里程计精度评估，地图原文件未入库。
- [10 月 5 日](daily/2026-10-05.md)：AMCL 初始位姿、yaw/四元数、TF 实时查询与 NavigateToPose Client。
- [10 月 6 日](daily/2026-10-06.md)：单点导航、FollowWaypoints 与 A/B/C/D 巡检有运行完成反馈；到点通过 SpeechText Service 播报，并从 Image Subscription 缓存图像，经 cv_bridge/OpenCV 保存照片。

巡检链路为 `FollowWaypoints → 到点事件 → SpeechText + 当前图像保存`。最后一个点需在成功 Result 中补处理，不能只看 Feedback 索引变化。已记录 frame 拼写、角度单位、重复 Goal、接口/构建依赖、bridge 与图像判空等排错。最终源码、每点一次触发、多圈巡检和异常恢复证据仍未完整归档。

### 8. pluginlib、Costmap 与 A*：10 月 8–11 日

[10 月 8 日](daily/2026-10-08.md) 学习插件导出、共享库、XML、GlobalPlanner 生命周期；[10 月 9 日](daily/2026-10-09.md) 记录 StraightLinePlanner 被加载、直线路径及 MPPI 跟踪反馈，并读取真实工作区源码、重新编译运行固定地图的八方向 A*。

算法学习覆盖 `g/h/f`、优先队列、closed、parent 回溯、四/八邻域、Octile 启发式、斜向代价和禁止切角；[10 月 10 日](daily/2026-10-10.md) 记录 `astarSearch()` 封装、不可达和起终点重合测试、索引修正及 AStarPlanner 框架构建/发现。

集成路线是 `Costmap 快照 → 世界/栅格坐标转换 → A* → nav_msgs/Path → Controller → 底盘`。10 月 11 日学习者确认第八章结束并转入视觉方向，但插件被发现、固定地图搜索通过与完整导航边界测试是不同结论。最终插件源码、搜索取消、重规划、footprint 碰撞及靠障路径优化仍需核验。

### 9. RGB-D 与 RTAB-Map：10 月 11–14 日

| 日期 | 已观察到的进展 | 未通过的部分 |
|---|---|---|
| [10-11](daily/2026-10-11.md) | RGB/Depth、点云桥接及 RViz 显示有完成反馈；临时补传感器 TF 后可见 | 正式 optical TF、最终相机/bridge/launch 未同步 |
| [10-12](daily/2026-10-12.md) | 使用轮式 `/diff_drive_controller/odom` 的 RTAB-Map 可显示 3D Map，测试更复杂场景 | 平移后重影、条纹、错位仍存在；当天关闭 visual/ICP odometry，不能称为纯视觉里程计实验 |
| [10-13](daily/2026-10-13.md) | 切换 RGB-D VO，`/rgbd_odom` 曾发布，完整 TF 链曾出现 | `0/20` 内点、`quality=0`、协方差对角线曾为 `9999`；动态 odom TF 间歇缺失 |
| [10-14](daily/2026-10-14.md) | 改用 `/camera_odom`；移除错误空 launch 参数后进入视觉处理，保存 4 张原始截图 | 初始化仅 5/20 特征、0/20 或 7/20 内点、投影越界、无 guess 重试失败；运动后失跟/漂移未解决 |

**截至收尾，RGB-D 传感器完整验收和视觉建图/定位验收均未完成。** 节点运行、有 odom 消息频率、偶尔出现正 quality 或能看到点云，都不能代替连续有效跟踪及地图精度验证。根因尚未确定，不能断言只需更换场景、降低 MinInliers 或调整某个 TF 即可修复。

## 环境与运行入口

### 环境范围

历史 TF 环境记录为 Ubuntu 24.04.5 LTS / Parallels / aarch64 / ROS 2 Jazzy；10 月 2 日及后续仿真记录补充 Gazebo Harmonic。`ENVIRONMENT.md` 中仿真器一栏仍较旧，具体版本、编译器、CMake 和 Python 版本应在复现机器重新确认。

技术栈包括 C/C++17、POSIX/pthread、Make/CMake、Python、ament/colcon、rclcpp/rclpy、TF2、URDF/Xacro、RViz2、ros_gz、gz_ros2_control；后期实验使用 slam_toolbox、Nav2/AMCL/MPPI、pluginlib、OpenCV/cv_bridge、espeak-ng 与 RTAB-Map。**它们不是全部已打包安装的本仓库依赖**；ROS 工作区先按各自 `package.xml` 和快照说明配置，包声明未保证覆盖所有历史实验。

本次 README 整理在 macOS 进行，未重新运行 Linux 并发/网络实验、ROS 2、Gazebo、Nav2 或 RTAB-Map。以下为已归档源码的入口，不是本次全栈测试通过声明。

### 获取与仓库检查

```bash
git clone https://github.com/KiloMing/LINUX-STUDY.git
cd LINUX-STUDY
python3 scripts/validate_repository.py
python3 -m unittest discover -s tests -v
```

检查器覆盖必要文件/目录、状态字段、本地 Markdown 链接及 Git 跟踪的 ELF、`.o`、`.d`；测试针对检查器本身。通过这些检查不证明机器人功能或学习验收通过。

### DAY2 多文件 C++ 示例

从仓库根目录进入；需要 make 和支持 C++17 的 g++：

```bash
cd DAY2/Project_demo
mkdir -p build bin
make
./bin/app
```

预期依次打印 `hi`、`hello world`、`linux`。必须先建输出目录，因为原 Makefile 未实现目录创建规则。

### TF C++ 快照

在依赖已准备好的 Linux/Jazzy 环境，从仓库根目录进入工作区：

```bash
cd ros2/tf_test
source /opt/ros/jazzy/setup.bash
colcon build --packages-select demo_cpp_tf
source install/setup.bash
ros2 run demo_cpp_tf tf_static
```

另开终端，进入同一工作区并 source 两份环境后运行 `ros2 run demo_cpp_tf tf_dynamic`。第三个同样配置的终端分别执行：

```bash
ros2 run tf2_ros tf2_echo map target_point
ros2 run tf2_ros tf2_echo map base_link
ros2 run tf2_ros tf2_echo base_link target_point
```

每个 echo 持续运行，Ctrl+C 后再执行下一个。历史第三组平移约 `(2.598,-1.500,-2.000)`、yaw 30°。完整说明见 [tf_test](ros2/tf_test/README.md)，listener 草稿不作为可执行目标。

### Gazebo / ros2_control 旧快照

从仓库根目录进入，确认 Jazzy、Gazebo Harmonic、ros_gz、gz_ros2_control、Xacro 等依赖可用：

```bash
cd ros2/chapt6_ws
source /opt/ros/jazzy/setup.bash
colcon build --packages-select mybot_description
source install/setup.bash
export GZ_IP=127.0.0.1
ros2 launch mybot_description sim_control.launch.py
```

该 launch 用同一 Xacro 发布机器人描述、加载 `my_sensor_world.sdf`、创建机器人并桥接 `/clock`。归档 YAML **只有 controller_manager 的 update_rate/use_sim_time**，没有完整 diff_drive 控制器配置或自动激活流程；launch **没有 `/scan` 和相机桥接**。Gazebo 中出现传感器话题不等于 ROS 中已可用，因此这不是完整建图/导航启动命令。

launch 保留虚拟机软件渲染设置；`gui:=false` 仅关闭 GUI，不保证传感器渲染无需显示环境。旧 `display_robot.launch.py` 默认引用的 `first_robot.urdf` 不在仓库，模型显示另见 `display2_robot.launch.py`。详细边界见 [工作区说明](ros2/chapt6_ws/README.md)。

本仓库不提供假定存在的 A*、巡检或 RTAB-Map 一键启动命令；先取得 Ubuntu 最终源码、地图、配置和依赖版本，才可形成对应复现入口。

## 已知问题与尚缺的验收

| 范围 | 当前问题或证据缺口 |
|---|---|
| DAY3 | `test2.c` 第二次读 pipe 时写端仍打开，可一直阻塞；`myls.c` 使用个人绝对路径和 `stat`，符号链接识别/递归边界需改进；文件复制未完整处理读失败、短写与 EINTR |
| DAY4 | Mini Shell 只执行一条命令，缺空输入校验、引号/管道/重定向和内建命令；不能当作成熟 Shell |
| DAY5 | FIFO reader 在 EOF 后不退出，且可能打印未终止的 buffer；`mmap.cpp` 映射 256 字节却按旧文件长度解除映射；writer 的嵌套 if/错误检查需修正；共享映射不自动提供同步 |
| DAY6 | mutex 未初始化且未设置 `PTHREAD_PROCESS_SHARED`，放进共享内存不等于完成跨进程同步；mmap/semaphore 错误检查和资源清理仍需补齐 |
| 并发/网络 | 3P2C、条件变量和 rwlock 的独立/延迟复测；短读写、EINTR、完整 ACK、子进程退出与回收、空/大/含零字节文件一致性、协议序列化等未完整验收 |
| ROS 2 基础 | Turtle Patrol 独立闭环、语音线程解耦、Service/Parameter 异常路径及最终源码；TF listener 草稿未完成 |
| 模型/仿真 | 旧快照轮轴沿 Y，而圆柱惯量宏按 Z 主轴；未见最终修复证据，也未证明它就是扫描重影根因；最新控制器/传感器桥接/20 Hz LiDAR 与地图未同步 |
| Nav2 / A* | 路径靠障、完整 footprint、狭窄通道、切角/非法起终点、搜索取消与周期重规划；`GoalOccupied`、索引初始化最终修复需日志/源码核验 |
| 速度与 Action | `Velocity command timed out. Braking.`、时间戳及 Goal 生命周期异常原因未定，不直接归因于 A* 或 waypoint_follower |
| 巡检 | 多圈索引重置、最后点补触发、每点只播报/拍照一次、照片覆盖策略；服务/相机缺席、aborted/canceled 和写盘失败等需测 |
| RGB-D | 跟踪失效、动态 TF 中断、重影/漂移；外参、RGB-D 配准、内参、同步、guess 来源、纹理与运行性能尚待逐项定位 |
| 真机 | Gemini Pro 仅有深度 USB 识别的历史引用，RGB/驱动/对齐未验收；FET3588 系统/BSP、实体底盘和完整部署无通过证据 |

以上保留为收尾时的真实问题清单。本次文档提交没有顺手修复旧代码，也没有修改掌握度等级。

## 后续方向与验收顺序

1. **补齐归档。** 从 Ubuntu 真正运行版本逐文件 diff 后同步第七、八章和 RGB-D 的源码、launch、YAML、Xacro、world、地图/数据库说明及日志；避免以仓库旧版本覆盖新工程。
2. **先恢复有效视觉跟踪。** 静止时连续记录 quality、lost/features/matches/inliers、协方差及 TF；核查实际 TF 发布者、`enable_odom_tf`、`guess_frame_id` 和 `Odom/GuessMotion`，再检查光学外参、RGB-D 配准/内参、同窗口时间戳和帧率。静止稳定后分开做低速前后/左右转，并与 Gazebo 真值比较。
3. **完成 RTAB-Map 建图/定位。** 验证地图几何、轨迹和回环，保存独立数据库，完整重启后加载并重定位。保留轮式 odom 基线；不以临时静态 odom TF 掩盖动态跟踪失败。
4. **再接视觉 Nav2。** 先用官方规划器建立基线，再对比自定义 A*；验证二维占据地图、深度障碍输入、footprint、停车/绕障及异常恢复。三维点云可视化不自动等于导航地图，SLAM + Nav2 也不自动提供自主探索。
5. **逐级迁移真机。** Gemini Pro 数据和驱动 → FET3588 系统/依赖及性能 → MCU 通信、编码器、轮速 PID、odom 与实体底盘。每级单独验收；底盘先完成限速、停车、失联停车和急停，再联调导航。
6. **补独立能力证据。** 按 [MASTERY](MASTERY.md) 完成脱离答案的复现、需求变式和延迟复测；继续补网络、并发和工程化基础，不以课程进度替代能力验证。

## 提交历史中的关键节点

以下日期按仓库记录列出；提交说明是历史线索，完成程度仍以相应文件中的证据边界为准。

| 时间 | 里程碑 | 提交 |
|---|---|---|
| 08-21 | DAY1 起步 | [d28dff8](https://github.com/KiloMing/LINUX-STUDY/commit/d28dff8958) |
| 08-24～08-30 | Makefile、文件/进程、IPC、sem/mutex | [316b83b](https://github.com/KiloMing/LINUX-STUDY/commit/316b83b05e) → [1dc2bde](https://github.com/KiloMing/LINUX-STUDY/commit/1dc2bdee3c) |
| 09-06 | 重组文档、建立检查器、移除跟踪的构建产物 | [bc959ad](https://github.com/KiloMing/LINUX-STUDY/commit/bc959ad4f1) / [bad910f](https://github.com/KiloMing/LINUX-STUDY/commit/bad910ffa5) / [ad17650](https://github.com/KiloMing/LINUX-STUDY/commit/ad17650c1f) |
| 09-18～09-20 | TCP 文件传输、select、poll/epoll LT 记录 | [7bd2ea3](https://github.com/KiloMing/LINUX-STUDY/commit/7bd2ea3f78) / [6b86339](https://github.com/KiloMing/LINUX-STUDY/commit/6b86339354) |
| 09-28 | 撤回旧 Patrol 摘要，依据实际工程重建记录 | [ea8a528](https://github.com/KiloMing/LINUX-STUDY/commit/ea8a528288) / [ba1009e](https://github.com/KiloMing/LINUX-STUDY/commit/ba1009e31c) |
| 09-29～10-02 | TF 与 Gazebo/控制源码证据归档 | [911a588](https://github.com/KiloMing/LINUX-STUDY/commit/911a588efe) / [d1dceaf](https://github.com/KiloMing/LINUX-STUDY/commit/d1dceafaa1) |
| 10-04～10-06 | 建图、定位、导航与巡检记录 | [6fd292a](https://github.com/KiloMing/LINUX-STUDY/commit/6fd292aeac) / [6140bb7](https://github.com/KiloMing/LINUX-STUDY/commit/6140bb7d21) |
| 10-08～10-11 | pluginlib/A*、第八章收尾和 RGB-D 路线 | [b550c31](https://github.com/KiloMing/LINUX-STUDY/commit/b550c31cf8) / [3f36d27](https://github.com/KiloMing/LINUX-STUDY/commit/3f36d27d42) |
| 10-12～10-14 | RTAB-Map 地图异常、VO/TF 失效和截图归档 | [539f100](https://github.com/KiloMing/LINUX-STUDY/commit/539f100a7b) → [256e27b](https://github.com/KiloMing/LINUX-STUDY/commit/256e27b6858f58b94a7701d96d324c86c66b9bc9) |

本轮收尾的成果是把学习内容、可用源码、历史运行结果和未完成事项放到同一个可追溯入口。未来若继续学习，仍应以新的源码、配置和实验结果更新记录。

## 许可说明

仓库根目录未提供统一 LICENSE。`mybot_description` 有自己的 [Apache-2.0 许可证文件](ros2/chapt6_ws/src/mybot_description/LICENSE)，`demo_cpp_tf/package.xml` 的许可声明仍是 TODO；不能据此把整个仓库统一声明为 Apache-2.0。
