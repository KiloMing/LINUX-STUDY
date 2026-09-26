# CURRENT

最后更新：2026-09-26

## 当前进度快照

当前主线为 **ROS2 Topic 实践 → 第 4 章 Service**，停在 **4.1.1 服务通信介绍**。今天已学习 turtlesim 闭环控制、status_interfaces 自定义消息、status_publisher Python 发布，以及 status_display C++/Qt 订阅显示。

**下一步从 Service 继续：Topic 是持续广播；Service 是 Client 请求一次、Server 处理并返回一次结果。** 先巩固 Client / Server / Request / Response，再做最小调用。Parameter 只曾被提及，不记为已完成 4.1.2。

## 仓库已验证

- 本仓库已保存 [2026-09-26 每日记录](daily/2026-09-26.md)，覆盖六项学习内容、问题与后续验证边界。
- [9 月 25 日 CycleContarl 旧草稿](ros2/2026-09-25-cycle-contarl-UNVERIFIED.md) 继续保留 UNVERIFIED；本次没有取得最终 ROS2 源码或重新执行构建。
- 既有 Linux/Socket/Topic 证据见 [9 月 25 日记录](daily/2026-09-25.md) 及其前序每日记录；历史证据不等于本次独立复测。

## 学习者自述与对话记录

- 闭环内容已推进到 atan2、角度误差归一化、角度阈值、距离比例控制与最大速度限制；最终到点行为仍需源码与运行证据。
- status_interfaces 接口生成与查询、status_publisher 的 /sys_status 发布和 topic echo 已跑通（按学习记录）。
- hello_qt、sys_status_display 已跑通；对话文字记录 Qt 窗口显示主机、CPU、内存和网络状态。本次未重新核验历史截图。
- 已理解用 std::thread 跑 rclcpp::spin，主线程跑 app.exec；GUI 跨线程直接更新需改为主线程处理，不能将“能显示”当作线程安全证据。
- Topic 与 CMake 维持 L2；没有新增闭卷独立复现，Service 仅建立基础概念。

## 待验证

- 客户端先检查 `argc`，正确处理 `stoi` 异常和端口范围。
- `send()` 返回值使用 `ssize_t`；`send_all()` / `recv_all()` / 文件写入处理短读写和 `EINTR`。
- 客户端累计接收完整的 2 字节 ACK；服务端检查 `recv_file()` 返回值，子进程执行 `close(client_sock) + _exit()`。
- 空文件、大于 4096 字节文件、包含 `0x00` 的二进制文件逐字节一致；多客户端同时连接，子进程正常回收。
- 明确结构体序列化和 `uint64_t` 字节序，增加最终完成 ACK、校验和和失败文件清理。
- producer-consumer 闭卷独立复现、3P2C 最终运行证据及 rwlock 源码/输出仍待补齐。

## 当前问题/待解决

- 9 月 25 日旧草稿遗留核验（9 月 26 日已学习阈值和限幅，最终源码尚未核验）：将 `cmd_vel.linear.y = k_ang * error_ang` 修正为 `angular.z`；确认 `<cmath>`/`M_PI`；核对闭环 executable 的 CMake 依赖与安装。`target_ang_` 可局部化，`max_speed_` 尚未实际限幅。

- A) cpp-httplib 头文件引用路径尚需在实际工程中验证。对话中的目录是 `include/cpp-httplib/httplib.h`；视频的 `include_directories(include)` 配套 `#include "cpp-httplib/httplib.h"`，当前 `<httplib.h>` 则要求搜索起点指向 `include/cpp-httplib`。
- B) 需要继续理解 `start_download` 的线程创建与 callback 生命周期；实际源码未核验，不能直接认定异步方式或引用安全。
- C) Timer Publisher → Topic → Subscription 已实际跑通；仍需用 `ros2 topic info/echo/hz` 保存 CLI 观察证据，并从空白独立复现一次。
- D) espeak-ng 已接入 Subscription 并完成语音输出；已解决普通 library 链接与 plain/keyword signature 冲突。当前 queue + `speech_thread` 解耦代码尚待实际运行验证。
- E) `ROS_DISTRO`、Ubuntu 版本和 arch 均待实际学习环境命令证据；[ENVIRONMENT.md](ENVIRONMENT.md) 保持待确认。

## 下一次测试

1. 先口述 Client / Server / Request / Response，并对比 /spawn 与 /turtle1/pose。
2. 从 4.1.1 做一次最小 Service 查询与调用，记录接口及返回结果，再进入 Parameter。
3. 并行补存 status 三个包的源码、构建、接口查询、topic echo 与 GUI 证据，独立复现发布/订阅。
4. 核验闭环 angular.z、角度跨界、阈值、最大速度和到点停止；核验 GUI 主线程更新、shutdown/join 与窗口退出。
5. 保留 speech_thread 运行/退出、CMake 构建链、Socket/TCP 和 producer-consumer/rwlock 的独立验收；历史清单见 [9 月 25 日记录](daily/2026-09-25.md) 及此前 daily。

## 下一步

**继续第 4 章 Service 的 4.1.1；先请求/响应，再后续参数内容。** 课程推进与独立能力验收分开记录，既有待验证项继续并行补证，不阻断本次已明确的续学入口。长期路线和评级标准保持不变。
