# CURRENT

最后更新：2026-09-10

## 当前阶段

Linux 系统编程收尾与并发基础验证；当前使用 C++ + pthread，已从有界 `std::queue<int>` 推进到 2 Producer + 2 Consumer 的设计和结束协议，尚未独立实现并编译运行。

## 仓库已验证

- DAY1–DAY6 保存 Linux 基础、Git/Makefile、文件 I/O、目录操作、pipe、进程、`fork/exec/wait`、Mini Shell、FIFO、`mmap`、semaphore、mutex 的笔记或源码练习。
- 2026-09-08 的学习事实见 [daily/2026-09-08.md](daily/2026-09-08.md)。独立源码仓库 [KiloMing/LinuxCodeSrc](https://github.com/KiloMing/LinuxCodeSrc) 的 `main` 已有单槽 producer-consumer 源码和历史运行截图；2026-09-09 收尾检查时工作区干净并与 `origin/main` 同步。
- `LinuxCodeSrc` 中没有 2026-09-09 讨论的容量 5 `std::queue<int>`、2 Producer + 1 Consumer 版本，因而本次没有新增 queue 代码或运行结果证据。
- “仓库已验证”只表示存在学习证据，不等于已经达到独立实现等级。

## 学习者自述

- 已有单槽循环版本的实现和运行经历，尚未完成脱离答案的延迟复现；9 月 9 日的 FIFO、数量不匹配挂起讨论见 [daily/2026-09-09.md](daily/2026-09-09.md)。
- 今天继续 2 Producer + 2 Consumer 设计，复习 wait 原子释放 mutex 并等待、唤醒后重新竞争 mutex、拿锁后才返回；`while` 既防虚假唤醒，也防其他线程先改变条件。
- 引入 `producers_done` 和 `PRODUCER_COUNT`，不再依赖固定 `TOTAL = 20`。能答出所有 Producer 结束但 buffer 仍有数据时应继续消费；完整结束协议仍需提示。
- 已讲解每个 Producer 完成后在锁内递增计数，最后一个 Producer `broadcast(not_empty)` 唤醒所有等待 Consumer，自行检查退出条件。Producer 完成不等于整个任务完成。
- 已讨论 4 个 `pthread_t`、先 create 全部线程再 join，避免先等 Producer 而 Consumer 尚未创建时因满队列永久等待；create 顺序不保证执行顺序，mutex 保护共享 buffer 和状态。

今天的实际回答与薄弱点见 [daily/2026-09-10.md](daily/2026-09-10.md)。没有独立编译运行 2P2C，也没有新增源码验证；producer-consumer 保留 L2，condition variable 和 rwlock 仍待验证。

## 待验证

- 独立补条件时仍混淆 `buffer.empty()`、固定 `count <= 20` 和 `producer_count`，尚不能独立实现完整 2P2C。重点复测：
  - Producer wait：`buffer.size() >= MAX_SIZE`，等待 `not_full`。
  - Consumer wait：`buffer.empty() && producers_done < PRODUCER_COUNT`，等待 `not_empty`。
  - Consumer 退出：`buffer.empty() && producers_done == PRODUCER_COUNT`。
  - Producer push 后 `signal(not_empty)`；Consumer pop 后 `signal(not_full)`。
- broadcast / destroy 名称曾混淆，需区分唤醒所有等待线程和线程结束后的资源清理。
- 完整有界队列的独立实现、空满等待、数据不丢不重、结束协议及线程退出仍待实际编译运行验证。旧单槽 `full` / `data` 遗留变量和 Producer 数据范围问题应在独立实现时处理，不记为已经修正。
- rwlock 独立复现及互斥关系验证仍未完成；此前记录的 `LinuxCodeSrc/DAY7/src.cpp` 对未创建 `tid4` 调用 join 的问题，本次没有重新检查或修正。
- 后续分别定位已有 mutex、`mmap`、FIFO reader 和 Mini Shell 练习中的问题。

一次答错先记录为复测项，不直接判定为长期知识缺口。

## 下一次测试

1. 先做 5–10 分钟短复盘：不看答案写等待/退出条件和通知方向，解释 wait、while、broadcast / destroy；推演 Consumer 的三种状态。
2. 从空文件独立写出完整 2 Producer + 2 Consumer 有界队列，采用 `producers_done` 结束协议；四个线程全部 create 后再 join，不按固定 TOTAL 分配 Consumer 消费次数。
3. 实际编译运行并保存代码和输出：检查数据恰好消费一次、最终队列为空、全部线程退出，验证空满等待；改变生产次数后再验证结束协议，不把一次调度顺序当作保证。
4. 之后补 rwlock 的独立实现和验证：先完成 2 Reader + 1 Writer，再用多 Writer 变式验证写写互斥，检查未创建线程的 join 问题。

## 下一步

先完成短复盘、2P2C 独立实现与实际编译运行，再补 rwlock 独立验证。未完成这些前不进入 Socket；保持 [ROADMAP.md](ROADMAP.md) 既定路线及 condition variable、rwlock 至少 L3 的阶段门槛。
