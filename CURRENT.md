# CURRENT

最后更新：2026-09-25

## 当前阶段

当前主线为 **ROS2 turtlesim Topic 发布/订阅 → 闭环到点控制框架**。2026-09-25 创建 `demo_cpp_topic`，练习 Twist 定时发布画圆、Pose 订阅打印及 CMake 多 executable；发布/订阅分离程序已有学习者“成功”的反馈，本次没有重新运行验证。

最新 `CycleContarl` 已在同一 Node 内组合 Publisher + Subscription，并写出 sqrt 距离、atan2 目标角、角度归一化与双 P 控制草稿。**尚未进行 colcon 编译/运行验证，不能记为闭环控制通过或已掌握。** 目标 `(1.0, 1.0)`，`k_dis/k_ang` 均为 1.0，`max_speed_` 声明为 3.0 但未用于限幅。见 [今日记录](daily/2026-09-25.md) 和 [UNVERIFIED 原始代码快照](ros2/2026-09-25-cycle-contarl-UNVERIFIED.md)。

2026-09-24 Topic 收发和 espeak-ng 语音输出的已有证据仍保留；queue + `speech_thread` 线程版仍待最终运行和退出验证。Topic 与 CMake 维持 L2，不因今日草稿升级。

Socket 的 poll/epoll LT、3 客户端独立运行与 TCP 文件传输健壮性，以及已有并发练习验收继续并行补证。

## 仓库已验证

- `daily/2026-09-15.md` 记录最小 Echo Server 已实际跑通，以及当时 TCP 合并读取现象。
- 独立源码仓库 [KiloMing/LinuxCodeSrc](https://github.com/KiloMing/LinuxCodeSrc) 的提交 [`bebcefb`](https://github.com/KiloMing/LinuxCodeSrc/commit/bebcefb7c156a968e79fcfc4153c12d1cee39524) 含 `20260918/client_file.cpp`、`server_file.cpp`、115 字节的 `aaa.txt` 与相同内容的 `recv_aaa.txt`。
- 2026-09-19 源码仓库提交 [`e4da15c`](https://github.com/KiloMing/LinuxCodeSrc/commit/e4da15c4df4fea2fc0d4bc736973c1ae7b17f1ae) 新增 `20260919/select_server.cpp`，记录 Select Server 学习版实现；同一提交还把 9 月 18 日文件传输代码中若干 `send()` 返回值和接收累计变量改为 `ssize_t`。
- 2026-09-20 源码仓库新增 `20260920/poll_server.cpp` 与 `20260920/README.md`，记录 poll 多客户端主干以及 epoll LT 入门模型；均标记为 guided learning evidence，不作为独立 L3 证据。
- 本学习仓库保存学习事实、理解、问题和复测计划；可执行源代码继续放在 `LinuxCodeSrc`。今日仅以 Markdown 代码块保存未验证聊天快照，不作为构建目标。
- 2026-09-24 对话运行截图验证 `demo_cpp_pkg` 的 `timer` 与 `subscription` 已成功构建、安装并运行，Publisher/Subscription 能持续收发 Topic 消息；随后 Topic → espeak-ng 语音输出也已跑通。
- “仓库已验证”只表示证据存在，不等于已经达到闭卷独立实现等级。

## 学习者自述与对话记录

- 2026-09-25 发布画圆与 Pose 打印练习后明确回复“成功”；最后贴出的 `CycleContarl` 明确未编译、未运行。详见 [今日记录](daily/2026-09-25.md)。

- 2026-09-18 文本文件传输已经成功；客户端在服务端返回 `"OK"` 后分块发送，服务端按 `file_size` 保存为 `recv_` 前缀文件。
- 能解释 `server_sock` 用于监听、`client_sock` 用于具体连接；fork 后父进程关闭连接 fd、子进程关闭监听 fd，子进程完成后应关闭连接并退出。
- 理解 `open/read/send` 已经发送原始字节，TCP 不存在单独的文本/二进制发送模式；二进制正文长度取 `read()` 返回值，不能用 `strlen()`。
- 上述实现经过逐步提示和调试，尚不满足 L3 的独立复现要求。详见 [daily/2026-09-18.md](daily/2026-09-18.md)。
- 2026-09-19 已学习 select/bitmap/fd_set 并提交学习版 `select_server.cpp`。
- 2026-09-20 继续到 poll 并开始 epoll LT：能说明一个 `server_sock` 对应多个 `client_sock`，poll 的两次遍历分别用于扫描就绪事件与寻找空槽；开始理解 epoll 的注册模型、`epoll_ctl()` 和 `epoll_wait()`。尚无独立多客户端运行证据，详见 [当日记录](daily/2026-09-20.md)。

## 待验证

- 客户端先检查 `argc`，正确处理 `stoi` 异常和端口范围。
- `send()` 返回值使用 `ssize_t`；`send_all()` / `recv_all()` / 文件写入处理短读写和 `EINTR`。
- 客户端累计接收完整的 2 字节 ACK；服务端检查 `recv_file()` 返回值，子进程执行 `close(client_sock) + _exit()`。
- 空文件、大于 4096 字节文件、包含 `0x00` 的二进制文件逐字节一致；多客户端同时连接，子进程正常回收。
- 明确结构体序列化和 `uint64_t` 字节序，增加最终完成 ACK、校验和和失败文件清理。
- producer-consumer 闭卷独立复现、3P2C 最终运行证据及 rwlock 源码/输出仍待补齐。

## 当前问题/待解决

- 明日优先：将 `cmd_vel.linear.y = k_ang * error_ang` 修正为 `angular.z`；确认 `<cmath>`/`M_PI`；核对闭环 executable 的 CMake 依赖与安装。`target_ang_` 可局部化，`max_speed_` 尚未实际限幅。

- A) cpp-httplib 头文件引用路径尚需在实际工程中验证。对话中的目录是 `include/cpp-httplib/httplib.h`；视频的 `include_directories(include)` 配套 `#include "cpp-httplib/httplib.h"`，当前 `<httplib.h>` 则要求搜索起点指向 `include/cpp-httplib`。
- B) 需要继续理解 `start_download` 的线程创建与 callback 生命周期；实际源码未核验，不能直接认定异步方式或引用安全。
- C) Timer Publisher → Topic → Subscription 已实际跑通；仍需用 `ros2 topic info/echo/hz` 保存 CLI 观察证据，并从空白独立复现一次。
- D) espeak-ng 已接入 Subscription 并完成语音输出；已解决普通 library 链接与 plain/keyword signature 冲突。当前 queue + `speech_thread` 解耦代码尚待实际运行验证。
- E) `ROS_DISTRO`、Ubuntu 版本和 arch 均待实际学习环境命令证据；[ENVIRONMENT.md](ENVIRONMENT.md) 保持待确认。

## 下一次测试

优先完成 2026-09-26 接续测试：

1. 先闭卷 5–10 分钟解释 Pose → 误差 → Twist，说明 sqrt、atan2、[-pi, pi] 归一化及 P 控制；尚无测试结果。
2. 修正上述代码与构建配置，执行 `colcon build --packages-select demo_cpp_topic` 并留存输出。
3. 构建成功后 source 环境、查询 executable，运行 turtlesim 与闭环节点，关闭旧画圆发布器；验证是否向 `(1.0, 1.0)` 移动，记录 Pose、距离误差和到点判定。
4. 再实现限幅、观察控制效果与目标附近停止行为；最后独立复现/变式及延迟复测。

以下既有验收继续保留：

1. 闭卷解释 Timer 句柄/创建、Spin/Executor 与两种 callback 的事件来源，重点说明“注册 callback”为什么不等于“自动执行”。
2. 从空白独立完成最小 Publisher + Subscription，并用 `ros2 pkg executables`、`ros2 topic info/echo/hz` 保存运行与观察证据。
3. 故意制造 Topic 名不一致，解释为什么节点都能运行但不通信，并从 Publisher/Subscription count 定位问题。
4. 不看答案说明 `#include`、`find_library()`、`target_link_libraries()` 与 `ament_target_dependencies()` 的职责边界，复现一次 `undefined reference` 的判断过程。
5. 完成 queue + mutex + condition_variable + speech_thread 版本，验证接收与语音播放解耦；Ctrl+C 时线程必须能正常退出。
6. 保留 CMake 多 target/out-of-source 闭卷验收，以及 Socket、TCP 文件传输、producer-consumer/rwlock 遗留验收。

## 下一步

当前学习顺序为：**修正并编译闭环节点 → turtlesim 到目标点验证 → 限幅与控制效果观察 → Topic 独立复现/CLI 与语音工作线程遗留验收 → CMake 构建链补证 → Service → Action → Parameter/Launch**。今日顺序按学习者课程进展调整，未降低独立掌握门槛；cpp-httplib 与 Socket/并发待验证项继续保留。

Socket 不再继续扩展高并发服务器专项；已有 select/poll/epoll、TCP 文件传输和短读写等内容作为通用 Linux/通信能力并行补齐证据，不删除原验收要求。

长期路线见 [ROADMAP.md](ROADMAP.md)，教学与评级遵循 [TEACHING_PROTOCOL.md](TEACHING_PROTOCOL.md) 和 [MASTERY.md](MASTERY.md)。
