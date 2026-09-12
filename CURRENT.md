# CURRENT

最后更新：2026-09-12

## 当前阶段

Linux 系统编程收尾与并发基础验证，使用 C++ + pthread。3 Producer + 2 Consumer 已完成代码改写阶段，修正项仍需核对，尚无最终编译运行成功证据。闭卷独立复现未完成，producer-consumer 保留 L2。

## 仓库已验证

- DAY1–DAY6 保存 Linux 基础、Git/Makefile、文件 I/O、目录操作、pipe、进程、`fork/exec/wait`、Mini Shell、FIFO、`mmap`、semaphore、mutex 的笔记或源码练习。
- 2026-09-08 的学习事实见 [daily/2026-09-08.md](daily/2026-09-08.md)。独立源码仓库 [KiloMing/LinuxCodeSrc](https://github.com/KiloMing/LinuxCodeSrc) 的 `main` 已有单槽 producer-consumer 源码和历史运行截图；2026-09-09 收尾检查时工作区干净并与 `origin/main` 同步。
- `LinuxCodeSrc` 中没有 2026-09-09 讨论的容量 5 `std::queue<int>`、2 Producer + 1 Consumer 版本，因而本次没有新增 queue 代码或运行结果证据。
- “仓库已验证”只表示存在学习证据，不等于已经达到独立实现等级。

## 学习者自述与对话记录

- 2026-09-11 已编译运行 2P2C，程序正常退出，但参考过完整答案。
- 本次没有检查对应新源码或运行输出；上述结果按学习者提供的事实记录，不写成仓库源码验证。
- 2026-09-12 已完成 3P2C 代码改写阶段；对话片段暴露了参数、数据重复和结束广播等修正项，尚未提供修正后的完整源码和最终编译运行结果。详见 [daily/2026-09-12.md](daily/2026-09-12.md)。
- producer-consumer 保留 L2；condition variable、rwlock 仍待独立验证。此前运行记录见 [daily/2026-09-11.md](daily/2026-09-11.md)。

## 待验证

- 脱离答案写出等待/退出条件、通知方向及 `producers_done` 结束协议，区分 Producer 完成与队列消费完毕。
- 能解释 wait 原子释放锁并等待、返回前重新获取锁，以及 while 重检条件、broadcast 与 destroy 的区别。
- 正常退出不能单独证明数据不丢不重、空满等待和不同调度均正确；这些要在变式和闭卷实现中保存证据。
- rwlock 独立复现及互斥关系仍未验证；此前记录的 `LinuxCodeSrc/DAY7/src.cpp` 对未创建 `tid4` 调用 join 的问题，本次未重新检查或修正。
- 后续分别定位已有 mutex、`mmap`、FIFO reader 和 Mini Shell 练习中的问题。

一次答错先记录为复测项，不直接判定为长期知识缺口。

## 下一次测试

1. 核对并运行验证 3P2C：每个 Producer 生产 5 个，`BUFFER_SIZE = 3`，数据使用 `id*100+i`；删除遗留 `full`，完成计数在锁内更新，仅最后一个 Producer 广播。Consumer 等待/退出条件保持不变（见今日日报）。检查 15 个数据各消费一次、最终队列为空、3 个 Producer 和 2 个 Consumer 全部退出，保存源码与输出。
2. 随后闭卷从空文件独立复现有界 producer-consumer，实际编译运行。检查每个数据恰好消费一次、最终队列为空、全部线程退出，验证空满等待；保存代码和输出，并在隔天复测关键逻辑。变式中若看过答案，不作为 L3 证据。
3. 再独立完成 rwlock：2 Reader + 1 Writer 验证读读并发、读写互斥，增加第二个 Writer 验证写写互斥；检查未创建线程的 join 问题。

## 下一步

按“运行验证 3P2C → 闭卷从空文件独立复现 producer-consumer → rwlock 独立验证”继续，producer-consumer 独立验证通过、condition variable 与 rwlock 至少 L3 后再进入 Socket，不因路线更新提前推进。

长期路线见 [ROADMAP.md](ROADMAP.md)。2026-09-11 按学习者要求强化 Linux/C++、嵌入式和通信等通用工程能力，以综合机器人项目收尾，保留职业转向空间。按每周 8–12 小时、考试周约 5 小时安排，以验收推进；70% 通用核心 / 20% 机器人 / 10% 前沿探索按约 12 周滚动检查，同时做职业/行业校准。教学与评级仍遵循 [TEACHING_PROTOCOL.md](TEACHING_PROTOCOL.md) 和 [MASTERY.md](MASTERY.md)。
