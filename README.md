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

- 仓库证据覆盖 DAY1–DAY6。
- 当前可确认主题：Linux 基础、Git/Makefile、文件 I/O、进程、IPC、semaphore、mutex。
- 2026-09-11，2 Producer + 2 Consumer 已实际编译运行并正常退出；由于参考过完整答案，producer-consumer 保留 L2，不升 L3。
- 当前已进入 TCP/Socket 基础实践：Echo Server 与教学引导下的多进程文本文件传输已跑通，Socket 当前记录为 L2。
- condition variable、rwlock 及后续阶段均须通过独立任务确认，不直接写成“已掌握”。
- 原始笔记与源码保留原路径；发现的程序问题作为后续调试练习，不在整理时偷偷修正。

## 最新学习进度：2026-09-18

当天完成了多进程 TCP 文件传输实验，应用层通信顺序为：

```text
Client                                  Server 子进程
file_Info ────────────────────────────→ 接收文件名和大小
          ←──────────────────────────── 返回 "OK" ACK
文件正文 ─────────────────────────────→ 按 file_size 接收并保存
```

本次学习内容包括：

- 区分监听用的 `server_sock` 和具体通信使用的 `client_sock`。
- `fork()` 后父进程关闭 `client_sock` 并继续 `accept()`；子进程关闭 `server_sock`，处理完成后关闭连接并退出。
- 客户端通过 `argc/argv` 接收服务器 IP、端口和文件名，使用 `stoi()`、`htons()` 和 `inet_pton()` 配置连接。
- 使用 `file_Info { file_name[256], uint64_t file_size }` 传递文件元信息。
- 使用 `send_all()` / `recv_all()` 处理 TCP 短写和短读的基本问题。
- 客户端收到完整 `"OK"` 后，使用 `open/read/send_all` 分块发送文件。
- 服务端使用 `recv_file/open/write`，按 `file_size` 收满正文并保存为 `recv_` 前缀文件。
- 理解 TCP 传输的是原始字节流，`open/read/send` 可以传输文本和二进制文件；正文长度必须使用 `read()` 的实际返回值。
- 排查了 `stoi invalid_argument`、`bind: Address already in use`、`c_str()`、`send()` 返回值类型和忘记调用 `recv_file()` 等问题。

当天文本文件已经实际传输成功。实现过程使用了逐步教学和调试提示，因此当前按 L2 记录；下一步是闭卷独立复现，并验证空文件、大于缓冲区的文件、含 `0x00` 的二进制文件和两个并发客户端。

完整学习过程、当前源码中仍待修正的问题和下一次测试见 [2026-09-18 学习记录](daily/2026-09-18.md)。Socket 阶段索引见 [socket/README.md](socket/README.md)。

实验源码位于独立仓库 [KiloMing/LinuxCodeSrc](https://github.com/KiloMing/LinuxCodeSrc) 的 [`20260918/`](https://github.com/KiloMing/LinuxCodeSrc/tree/bebcefb7c156a968e79fcfc4153c12d1cee39524/20260918) 目录。本仓库负责保存学习路线、掌握证据和复测计划，不重复保存源码。

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
