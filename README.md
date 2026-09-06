# LINUX-STUDY

从 Linux 系统编程出发，逐步走向 ROS 2、SLAM 与移动机器人实践的长期学习仓库。

这个仓库不只保存笔记，也保存学习路线、当前检查点、掌握证据和能够运行的工程。聊天内容可能变长或丢失上下文，因此学习进度以仓库记录为准。

## 当前事实

- 仓库证据覆盖 DAY1–DAY6。
- 当前可确认主题：Linux 基础、Git/Makefile、文件 I/O、进程、IPC、semaphore、mutex。
- condition variable、rwlock 及后续阶段均须通过实际任务确认，不直接写成“已掌握”。
- 原始笔记与源码保留原路径；发现的程序问题作为后续调试练习，不在整理时偷偷修正。

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
