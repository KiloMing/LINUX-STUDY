# MASTERY

这个文件记录“能做到什么”，不记录“看过什么”。等级必须由代码、口述、实验或调试结果支持。

## L0–L5

| 等级 | 含义 | 最低证据 |
|---|---|---|
| L0 | 未学习 | 没有相关证据 |
| L1 | 在提示下能识别术语 | 能指出概念用途，但不能独立解释 |
| L2 | 能解释模型并跟随完整示例 | 能说清主要流程并运行示例 |
| L3 | 能不照抄独立复现最小实现 | 有独立代码和运行结果 |
| L4 | 能修改需求并诊断常见失败 | 有变式任务和一次真实调试记录 |
| L5 | 能独立设计、论证、测试和评审集成方案 | 有综合项目、取舍说明、测试与复盘 |

## 升级规则

- 单次答对、看懂代码或成功运行现成示例，最多只能支持 L1–L2。
- L3 必须有脱离答案的独立复现。
- L4 必须同时有需求变式和调试证据。
- L5 必须在综合项目中说明输入、输出、依赖、约束、测试和失败模式。
- 一次答错不直接降级；使用去提示、变式和延迟测试确认是否是真正缺口。
- `学习者自述` 与 `仓库已验证` 分开记录；没有证据时写 `待验证`。

## 当前台账

以下是基于当前仓库能做出的保守判断，不代表完整能力测试。

| 主题 | 当前记录 | 依据/下一证据 |
|---|---|---|
| Linux 基础命令与权限 | L2 | `DAY1/README.md`；待闭卷操作验证 |
| Git 基础 | L2 | DAY1/DAY2 记录与仓库历史；待独立分支冲突练习 |
| Makefile | L2 | DAY2 工程；待从空目录独立复现并排错 |
| 文件 I/O 与目录 | L2 | DAY3 源码；待修正/扩展目录遍历练习 |
| 进程与 fork/exec/wait | L2 | DAY4 笔记和源码；待空输入等变式调试 |
| FIFO 与 mmap | L2 | DAY5 笔记和源码；待 EOF、长度和同步问题调试 |
| semaphore 与 mutex | L2 | DAY6 源码；待跨进程初始化问题调试 |
| condition variable | 待验证 | 2026-09-11 参考完整答案后 2P2C 编译运行并正常退出（学习者自述）；等待/退出条件、wait/while 与通知方向仍需闭卷验证，不升 L3 |
| read-write lock | 基础独立验证完成（学习者自述） | 2026-09-13 已独立编写并运行通过 2R2W；本次未核验源码/输出，待补存证据和延迟复测后确认 L3，不记为整个主题完成 |
| producer-consumer | L2 | 2026-09-11 的 2P2C 已编译运行并正常退出，但参考过完整答案（学习者自述，本次未核验新源码/输出）；2026-09-12 完成 3P2C 代码改写阶段，修正项待核对、最终运行待验证；闭卷独立复现未完成，不升 L3 |
| Socket | L2 | 2026-09-20 已从 select 推进到 poll，并开始 epoll LT；已有 guided poll Server 源码和 epoll 模型/API 讨论，但没有独立 3 客户端运行证据，也未闭卷独立完成 epoll LT。下一证据：独立完成 poll 多客户端 Echo Server，再独立完成 epoll LT Server；原 select、文件传输、短读写、二进制一致性与异常路径验收继续保留 |
| CMake | L2 | 2026-09-21 在提示下完成普通 CMake 与 ROS2 CMake 构建练习，能解释 target、`add_executable`、out-of-source build、`find_package` 与依赖链接，并真实定位多类配置错误；待从空目录闭卷完成多 target + library/依赖工程后再升 L3 |
| ROS 2 / TF2 / URDF / RViz | L1（ROS2 构建基础） | 2026-09-21 已创建 `ament_cmake` package，练习 `colcon build`、`source install/setup.bash`、`ros2 pkg executables` 与 `ros2 run`，但仍依赖提示且尚未独立跑通完整 node/topic 数据流；TF2/URDF/RViz 仍为 L0 |
| 运动学 / Odometry / 底盘集成 | L0 | 无仓库证据 |
| IMU / LiDAR / 状态估计 | L0 | 无仓库证据 |
| SLAM / Nav2 | L0 | 无仓库证据 |

## 2026-09-10 待复测

- Producer wait：`buffer.size() >= MAX_SIZE`，等待 `not_full`。
- Consumer wait：`buffer.empty() && producers_done < PRODUCER_COUNT`，等待 `not_empty`。
- Consumer 退出：`buffer.empty() && producers_done == PRODUCER_COUNT`。
- Producer push 后 `signal(not_empty)`；Consumer pop 后 `signal(not_full)`。
- 每个 Producer 在锁内更新完成计数，最后一个 `broadcast(not_empty)`；区分 broadcast 与 destroy，区分 Producer 完成与队列全部消费完。

2026-09-10 仍把 empty、固定 count <= 20 和 producer_count 混淆，未达到独立实现水平。这里记录待复习项，不因一次答错降级。后续进展见下方 2026-09-11 更新；历史详情见 [daily/2026-09-10.md](daily/2026-09-10.md)。

## 2026-09-11 更新

2P2C 已实际编译运行并正常退出，但参考过完整答案，不满足 L3 的独立复现要求。producer-consumer 保留 L2，condition variable、rwlock 仍待验证。本次未检查新源码和运行输出，详见 [daily/2026-09-11.md](daily/2026-09-11.md)。

下一证据：先做 3P2C 变式，再闭卷独立复现并保存运行结果，之后完成 rwlock 独立验证。condition variable、rwlock 至少 L3，且 producer-consumer 独立验证通过后再进入 Socket。

## 2026-09-12 更新

3P2C 已完成代码改写阶段，对话中已明确参数、数据区分和结束广播的修正要求；尚无修正后完整源码及最终编译运行成功证据，闭卷独立复现也未完成。producer-consumer 保留 L2，condition variable、rwlock 仍待验证，不新增独立掌握结论。详见 [daily/2026-09-12.md](daily/2026-09-12.md)。

下一证据：运行验证 3P2C → 闭卷从空文件独立复现 producer-consumer → rwlock 独立验证。

## 2026-09-13 更新

rwlock 2R2W 已独立编写并运行验证通过，记为基础独立验证完成（学习者自述）；本次未重新核验源码和输出，不扩展为变式调试或整个并发章节完成。Socket 已开始基础学习，保守记 L1，尚无完整示例运行或独立 client/server 的证据。producer-consumer 保留 L2，condition variable 的待验证项不变。

今天的实际进度替代上方历史记录中“rwlock 尚未独立验证、Socket 尚未开始”的当前状态判断；此前未完成的验收仍需补齐。详见 [daily/2026-09-13.md](daily/2026-09-13.md)。

## 2026-09-18 更新

多进程 TCP 文件传输已实际跑通，源码证据位于独立仓库 `LinuxCodeSrc` 的 `20260918/`。学习者能解释监听 fd / 连接 fd、父子进程关闭规则、`file_Info → OK → 正文` 协议、`send_all/recv_all` 与二进制字节流。实现经过连续教学和逐步调试，保守记为 L2，不认定闭卷独立实现。

下一证据：从空文件独立复现修正版；验证空文件、大于缓冲区的文本、含 `0x00` 的二进制文件和两个并发客户端；处理参数、短读写、子进程退出与回收，并完成一次协议序列化变式。详见 [daily/2026-09-18.md](daily/2026-09-18.md)。

## 2026-09-19 更新

TCP 基础已覆盖握手/挥手、缓冲区、Nagle 和 listen，正式进入 select/bitmap/fd_set 学习。源码仓库已提交学习版 `20260919/select_server.cpp`（`e4da15c`），但实现是在提示下形成，当前仍在巩固监听 fd、`accept()` 返回连接 fd 以及 `FD_SET()` 加入监控集合的关系，尚无独立多客户端运行证据，Socket 仍保持 L2。

下一证据：从空文件独立写出 select 多客户端 Echo Server（至少 3 个客户端收发、断连后清理并继续接受连接）+ 既有 TCP 文件传输闭卷复现；原有短读写、二进制一致性、异常路径、子进程回收和协议变式验收继续保留。详见 [daily/2026-09-19.md](daily/2026-09-19.md)。


## 2026-09-20 更新

I/O 多路复用从 select 推进到 poll，并开始 epoll LT。今天能解释 `pollfd.fd/events/revents`、`fd=-1` 空槽、监听 fd 与连接 fd 的一对多关系、poll 两次遍历的不同目的，以及“就绪不等于固定长度 TCP 消息完整到达”。源码仓库新增 `20260920/poll_server.cpp` 与 README，属于 guided learning evidence。

同日开始理解 `epoll_create1()`、`epoll_ctl(ADD/MOD/DEL)`、`epoll_wait()`，能说明为什么必须先判断返回 fd 是否为 `server_sock` 再决定 `accept()` 或 `read()`。尚无从空文件独立实现和多客户端运行证据，Socket 保持 L2。

下一证据：闭卷独立完成 poll 3 客户端 Echo Server并保存运行输出；再独立完成 epoll LT Server，之后进入非阻塞 + epoll ET。详见 [daily/2026-09-20.md](daily/2026-09-20.md)。


## 2026-09-21 更新

CMake 从 L0 进入 guided L2：能够解释 `add_executable(target sources...)`、多个源文件与唯一 `main()` 入口、多个 target 各自生成 executable、配置阶段与编译阶段、in-source / out-of-source build、`find_package()` 和 target 依赖传递。今天实际处理了未保存 CMakeLists、错误分号、依赖未先 `find_package`、源文件路径不一致以及源码目录内构建产生大量中间文件等问题。

ROS2 构建基础记 L1：已创建 `ament_cmake` package，开始理解 workspace/package/node、`ament_target_dependencies()`、`install(TARGETS ...)`、`ament_package()`、`colcon build`、环境 `source` 和 `ros2 run`。当前 `ros2 pkg executables demo_cpp_pkg` 已能列出已安装 executable，但曾因 CMake target 名与运行命令不一致出现 `No executable found`。没有闭卷独立 package/node 证据，也没有 topic/service/action 运行证据，不升 L2。

下一证据：从空 workspace 独立创建 package 和 rclcpp node，完成 build → source → executable 查询 → run；随后实现最小 publisher/subscriber，并保存运行输出。Socket 原有 poll/epoll 与 TCP 文件传输验收继续并行保留。

## 2026-09-22 更新

今天的 ROS2 Node/Topic、C++ Lambda/std::function/const 引用、cpp-httplib 下载 callback 与 CMake include path 讨论统一记为 **guided evidence**，依据是学习者提供的摘要及引导对话。没有新增闭卷源码或构建运行输出；CMake 保持 L2、ROS2 保持 L1，TF2/URDF/RViz 保持 L0，不新增独立掌握或章节完成结论。

Lambda/callback/httplib 暂不单独评级。下一证据：实际工程验证 include 路径；检查 start_download 的线程与 callback 生命周期；闭卷完成 CMake/ROS2 构建链，再进入 Publisher/Timer/Topic 实现。环境版本仍待命令证据，既有 Socket/并发验收保留。详见 [daily/2026-09-22.md](daily/2026-09-22.md)。
