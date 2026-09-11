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
| read-write lock | 待验证 | 未新增独立验证；producer-consumer 闭卷复现后做 2 Reader + 1 Writer 及多 Writer 变式，检查此前未创建 tid4 却 join 的问题 |
| producer-consumer | L2 | 2026-09-11 的 2P2C 已编译运行并正常退出，但参考过完整答案（学习者自述，本次未核验新源码/输出）；下一证据为 3P2C 变式、闭卷独立复现和运行结果，不升 L3 |
| Socket | L0 | 无仓库证据 |
| CMake | L0 | 无仓库证据；Makefile 不等于 CMake |
| ROS 2 / TF2 / URDF / RViz | L0 | 无仓库证据 |
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
