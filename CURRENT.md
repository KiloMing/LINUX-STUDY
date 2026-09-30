# CURRENT

最后更新：2026-09-30

## 当前进度快照

当前课程主线进入第六章：按学习者自述与今日学习反馈，6.2.1 URDF 基础、6.2.2 RViz 显示模型、6.2.3 Xacro 简化 URDF 已完成；Xacro 参数与 macro 已练习，RViz 黑屏已排查解决，robot_state_publisher / RViz 最终显示成功。当前在 6.2.4「创建机器人及传感器部件」开头，模块拆分尚未开始，精确播放秒数未知。

见 [今日记录](daily/2026-09-30.md)。本次未重新运行 ROS2/RViz，未取得今日模型源码和黑屏根因证据；独立掌握与延迟复现仍待验证，不上调 MASTERY。此前 TF 查询草稿见 [9 月 29 日记录](daily/2026-09-29.md)，Turtle Patrol 遗留项见 [9 月 28 日记录](daily/2026-09-28.md)。

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

- 9 月 25 日旧草稿遗留核验（9 月 26 日已学习阈值和限幅，最终源码尚未核验）：将 `cmd_vel.linear.y = k_ang * error_ang` 修正为 `angular.z`；确认 `<cmath>`/`M_PI`；核对闭环 executable 的 CMake 依赖与安装。`target_ang_` 可局部化；旧草稿的 `max_speed_` 未实际限幅，今日对话代码已出现限幅与 angular.z，但最终工程仍需归档核验。

- A) cpp-httplib 头文件引用路径尚需在实际工程中验证。对话中的目录是 `include/cpp-httplib/httplib.h`；视频的 `include_directories(include)` 配套 `#include "cpp-httplib/httplib.h"`，当前 `<httplib.h>` 则要求搜索起点指向 `include/cpp-httplib`。
- B) 需要继续理解 `start_download` 的线程创建与 callback 生命周期；实际源码未核验，不能直接认定异步方式或引用安全。
- C) Timer Publisher → Topic → Subscription 已实际跑通；仍需用 `ros2 topic info/echo/hz` 保存 CLI 观察证据，并从空白独立复现一次。
- D) espeak-ng 已接入 Subscription 并完成语音输出；已解决普通 library 链接与 plain/keyword signature 冲突。当前 queue + `speech_thread` 解耦代码尚待实际运行验证。
- E) TF 学习环境已确认 Ubuntu 24.04.5 / aarch64 / Jazzy，见 [ENVIRONMENT.md](ENVIRONMENT.md)。

## 下一次测试

1. 闭卷解释 URDF 的 link/joint/visual/geometry/origin，复现 robot_state_publisher → RViz 模型显示。
2. 解释 Xacro 参数、`${}`、macro 的定义与调用，修改尺寸并核对展开与显示结果。
3. 保存启动命令、模型和 RViz 状态，补齐黑屏排查的具体原因与修复证据。
4. 保留 TF 独立验收：解释父子 frame、四元数、sendTransform、lookupTransform 参数顺序与时间，确认动态 z=2 的实验意图。

## 下一步

从 6.2.4 开始创建 base、IMU、Laser、Camera 的独立 Xacro 模块，再由 fishbot.urdf.xacro include 并实例化总装，逐步验证显示。

5.3.3 C++ listener 在仓库中仍为未完成草稿，后续补齐 Buffer、TransformListener、timer、lookupTransform、try-catch、main 与 CMake，并对照 CLI 验证；没有新证据前不标记完成。Turtle Patrol、Service/Parameter/Launch 与历史独立验收继续保留。
