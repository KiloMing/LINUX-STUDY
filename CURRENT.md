# CURRENT

最后更新：2026-10-03

## 当前进度快照

**学习快照/当前课程进度：第六章 ros2_control 已收尾，已进入第七章 7.2.1；当前阻塞点是 SLAM 地图与实时 LaserScan 明显错位/重影。** 在问题解决前不进入 7.2.2 保存地图，避免影响后续 Navigation2 定位与规划。

今天完成 `joint_state_broadcaster`、`JointGroupEffortController` 与 `diff_drive_controller`，明确 effort 是旋转关节轴上的力矩目标、velocity 是轮速目标，并区分 command/state interface 与 claimed/unclaimed。`sim_control` / `gz_sim_control` 已整合 Gazebo、`robot_state_publisher`、`/clock` 与 `/scan` bridge、spawn 以及 joint state/diff drive 控制器自动激活。

已修复两条关键链路：`/clock` 重复 publisher 会使 `slam_toolbox` 报 `Detected jump back in time`，因此只保留一次时钟桥接；`scan_bridge` 必须使用 `additional_env=gz_env`，否则其 `GZ_IP` 与 Gazebo Server 不一致，Gazebo `/scan` 无法桥接到 ROS2。Gazebo 话题列举命令为 `gz topic -l`。

已安装并启动 `slam_toolbox`，理解 `map→odom→base_footprint→base_link→laser_link`、`/map`、`/scan`、OccupancyGrid 和 RViz `Fixed Frame=map`。原地旋转时 `odom→base_footprint` 的 x/y 基本不变、yaw 平滑，`map→odom` 基本稳定；这些证据仍不足以证明仿真真实运动与 odom 尺度一致。见 [今日学习记录](daily/2026-10-03.md) 与 [工程学习快照](ros2/chapt6_ws/README.md)。本次归档未直接登录 Ubuntu 工作区重新构建或同步源码，不上调 MASTERY。

今日运行结果依据学习者自述与当前会话归档，和 2026-10-02 已保存的源码/日志证据分开标注；没有把未在归档环境复跑的结果冒充为本次独立验证。

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

- SLAM 生成的 OccupancyGrid 与实时 LaserScan 有明显错位/重影。下一步先修 `wheel.urdf.xacro` 的轮子惯量方向：轮轴为 Y，但当前通用圆柱惯量按 Z 轴设置；此项尚未修改和验证。
- 修复惯量后，用 Gazebo ground-truth pose 与 odom 做 90° 原地旋转和固定距离直线对比。只有确认比例误差后，才分别校准 `wheel_separation` 或 `wheel_radius`；当前几何值 0.22 m / 0.032 m 与模型尺寸一致，暂不盲改。
- 重启全部仿真与 SLAM 进程，在静态且特征更丰富的环境中低速建图；RViz 和算法节点统一 `use_sim_time=true`。地图与实时扫描稳定重合后再保存地图。

- 9 月 25 日旧草稿遗留核验（9 月 26 日已学习阈值和限幅，最终源码尚未核验）：将 `cmd_vel.linear.y = k_ang * error_ang` 修正为 `angular.z`；确认 `<cmath>`/`M_PI`；核对闭环 executable 的 CMake 依赖与安装。`target_ang_` 可局部化；旧草稿的 `max_speed_` 未实际限幅，今日对话代码已出现限幅与 angular.z，但最终工程仍需归档核验。

- A) cpp-httplib 头文件引用路径尚需在实际工程中验证。对话中的目录是 `include/cpp-httplib/httplib.h`；视频的 `include_directories(include)` 配套 `#include "cpp-httplib/httplib.h"`，当前 `<httplib.h>` 则要求搜索起点指向 `include/cpp-httplib`。
- B) 需要继续理解 `start_download` 的线程创建与 callback 生命周期；实际源码未核验，不能直接认定异步方式或引用安全。
- C) Timer Publisher → Topic → Subscription 已实际跑通；仍需用 `ros2 topic info/echo/hz` 保存 CLI 观察证据，并从空白独立复现一次。
- D) espeak-ng 已接入 Subscription 并完成语音输出；已解决普通 library 链接与 plain/keyword signature 冲突。当前 queue + `speech_thread` 解耦代码尚待实际运行验证。
- E) TF 学习环境已确认 Ubuntu 24.04.5 / aarch64 / Jazzy，见 [ENVIRONMENT.md](ENVIRONMENT.md)。

## 下一次测试

1. 闭卷解释 command/state interface、claimed/unclaimed，以及 effort 与 velocity 的物理含义。
2. 修正轮子 Y 轴惯量并重新构建、完全重启；确认控制器、单一 `/clock` publisher 和 `/scan` 持续数据。
3. 保存 Gazebo ground-truth 与 odom 的 90° 原地旋转、固定距离直线对比数据，区分角度比例误差与距离比例误差。
4. 低速重新建图，检查实时 LaserScan 与 OccupancyGrid 墙体是否稳定重合；通过后再进入 7.2.2。
5. 保留 TF 独立验收：父子 frame、四元数、sendTransform、lookupTransform 与时间。

## 下一步

先解决 7.2.1 的地图/扫描错位：轮子惯量方向 → Gazebo ground-truth/odom 对比 → 必要时校准轮距或轮径 → 低速重建地图。只有实时扫描与地图稳定重合后，才进入 7.2.2 保存地图和后续 Navigation2。

第六章 `ros2_control` 已完成课程收尾：当前 launch 自动启动 joint state 与 diff drive 控制器，并只桥接一次 `/clock`；`scan_bridge` 与 Gazebo Server 统一继承 `gz_env`。仍应在每次完整重启后用控制器列表、publisher 详情、消息与 TF 做运行验收。

5.3.3 C++ listener 在仓库中仍为未完成草稿，后续补齐 Buffer、TransformListener、timer、lookupTransform、try-catch、main 与 CMake，并对照 CLI 验证；没有新证据前不标记完成。Turtle Patrol、Service/Parameter/Launch 与历史独立验收继续保留。
