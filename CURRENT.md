# CURRENT

最后更新：2026-09-22

## 当前阶段

当前主线为 **ROS2 基础 + C++ Lambda/callback/cpp-httplib/CMake include path**，承接阶段 3 CMake/C++ 工程化与阶段 4 ROS2 基础预热。今天根据下载示例补 C++ 回调与头文件路径前置知识，不代表阶段 3 已验收通过。

2026-09-21 已进行普通 CMake 与 ROS2 package 构建链入门实践；CMake 保留 guided L2，ROS2 保留 L1。2026-09-22 继续讨论 Node 与进程的区别、Topic 发布/订阅、Lambda 捕获/参数、std::function、const 引用和 HTTP 下载 callback。今天只有学习者摘要与引导讨论，没有新增闭卷代码或实际构建成功证据，详见 [当日记录](daily/2026-09-22.md)。

Socket 的 poll/epoll LT、3 客户端独立运行与 TCP 文件传输健壮性，以及已有并发练习验收继续并行补证。

## 仓库已验证

- `daily/2026-09-15.md` 记录最小 Echo Server 已实际跑通，以及当时 TCP 合并读取现象。
- 独立源码仓库 [KiloMing/LinuxCodeSrc](https://github.com/KiloMing/LinuxCodeSrc) 的提交 [`bebcefb`](https://github.com/KiloMing/LinuxCodeSrc/commit/bebcefb7c156a968e79fcfc4153c12d1cee39524) 含 `20260918/client_file.cpp`、`server_file.cpp`、115 字节的 `aaa.txt` 与相同内容的 `recv_aaa.txt`。
- 2026-09-19 源码仓库提交 [`e4da15c`](https://github.com/KiloMing/LinuxCodeSrc/commit/e4da15c4df4fea2fc0d4bc736973c1ae7b17f1ae) 新增 `20260919/select_server.cpp`，记录 Select Server 学习版实现；同一提交还把 9 月 18 日文件传输代码中若干 `send()` 返回值和接收累计变量改为 `ssize_t`。
- 2026-09-20 源码仓库新增 `20260920/poll_server.cpp` 与 `20260920/README.md`，记录 poll 多客户端主干以及 epoll LT 入门模型；均标记为 guided learning evidence，不作为独立 L3 证据。
- 本学习仓库只保存学习事实、理解、问题和复测计划；源代码继续放在 `LinuxCodeSrc`。
- “仓库已验证”只表示证据存在，不等于已经达到闭卷独立实现等级。

## 学习者自述与对话记录

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

- A) cpp-httplib 头文件引用路径尚需在实际工程中验证。对话中的目录是 `include/cpp-httplib/httplib.h`；视频的 `include_directories(include)` 配套 `#include "cpp-httplib/httplib.h"`，当前 `<httplib.h>` 则要求搜索起点指向 `include/cpp-httplib`。
- B) 需要继续理解 `start_download` 的线程创建与 callback 生命周期；实际源码未核验，不能直接认定异步方式或引用安全。
- C) 后续再进入 Publisher/Timer/Topic 的完整 ROS2 实现；目前只有 Topic 概念讨论。
- D) `ROS_DISTRO`、Ubuntu 版本和 arch 均待实际学习环境命令证据；[ENVIRONMENT.md](ENVIRONMENT.md) 保持待确认。

## 下一次测试

1. 先做 5–10 分钟闭卷复述：Node 与进程、Publisher → Topic → Subscriber、Lambda 的捕获/参数/返回类型、std::function 与 callback；记录真实回答。
2. 在实际学习环境保存 `printenv ROS_DISTRO`、`cat /etc/os-release`、`uname -m`，并记录 CMake/编译器版本。
3. 独立配对并验证两种 httplib include 方案，保存构建输出与 target include 参数；解释 CMakeLists.txt/Makefile、include path/find_package 的区别。
4. 检查实际 `download/start_download`，说明 `cb(path, response->body)` 的调用时机、执行线程、callback/引用/this 的生命周期和线程等待方式，再做最小实验。
5. 保留 9 月 21 日闭卷验收：从空目录建多 executable 的 out-of-source CMake 工程，解释唯一 main、依赖查找与 target 绑定；从空 workspace 创建 ament_cmake package 与最小 node，完成 build → source → executable 查询 → run，并定位一次 executable 名称错误。

通过构建链与回调前置验证后，再进入 Publisher/Timer/Topic 的完整实现。Socket 既有独立验收不取消。

## 下一步

当前学习顺序为：**CMake include path 实际验证 + Lambda/callback 生命周期 → CMake/ROS2 构建链闭卷复现 → Node 与 CLI → Publisher/Timer/Topic publisher/subscriber → Service → Action → Parameter/Launch**。进入 Topic 前必须先独立跑通 package 构建、安装、source 和 `ros2 run`，避免把 ROS2 当成黑盒。

Socket 不再继续扩展高并发服务器专项；已有 select/poll/epoll、TCP 文件传输和短读写等内容作为通用 Linux/通信能力并行补齐证据，不删除原验收要求。

长期路线见 [ROADMAP.md](ROADMAP.md)，教学与评级遵循 [TEACHING_PROTOCOL.md](TEACHING_PROTOCOL.md) 和 [MASTERY.md](MASTERY.md)。
