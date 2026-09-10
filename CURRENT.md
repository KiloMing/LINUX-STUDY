# CURRENT

最后更新：2026-09-09

## 当前阶段

Linux 系统编程收尾与并发基础验证；当前使用 C++ + pthread，正在从单槽 producer-consumer 推进到多线程、有界缓冲区版本。

## 仓库已验证

- DAY1–DAY6 保存 Linux 基础、Git/Makefile、文件 I/O、目录操作、pipe、进程、`fork/exec/wait`、Mini Shell、FIFO、`mmap`、semaphore、mutex 的笔记或源码练习。
- 2026-09-08 的学习事实见 [daily/2026-09-08.md](daily/2026-09-08.md)。独立源码仓库 [KiloMing/LinuxCodeSrc](https://github.com/KiloMing/LinuxCodeSrc) 的 `main` 已有单槽 producer-consumer 源码和历史运行截图；2026-09-09 收尾检查时工作区干净并与 `origin/main` 同步。
- `LinuxCodeSrc` 中没有 2026-09-09 讨论的容量 5 `std::queue<int>`、2 Producer + 1 Consumer 版本，因而本次没有新增 queue 代码或运行结果证据。
- “仓库已验证”只表示存在学习证据，不等于已经达到独立实现等级。

## 学习者自述

- 单槽 mutex + full + not_empty + not_full 的循环生产/消费 0～9 已实现并运行，但尚未完成脱离答案的延迟复现。
- 已从单槽模型推进到容量为 5 的 `std::queue<int>` 有界缓冲区，能说明 queue 是 FIFO，并纠正了把 `30, 20, 10` 当成 queue 取出顺序的错误。
- condition variable 已复测：`pthread_cond_wait()` 阻塞当前线程并原子释放 mutex；收到 signal 后线程仍须重新竞争 mutex，wait 返回前会重新获得 mutex。signal 不保存状态；`while` 同时处理虚假唤醒和线程醒来后条件已被其他线程改变的情况。
- 能说明 Producer 在 `push()` 后 `signal(not_empty)` 通知 Consumer，Consumer 在 `pop()` 后 `signal(not_full)` 通知 Producer。每次只释放一个槽位时通常用 signal 即可；broadcast 会带来额外唤醒，但 `while` 重检可维持正确性。
- 已写/讨论 2 Producer + 1 Consumer 共享同一个 queue：两个 Producer 共用 `producer(void*)`，通过 `void*` 参数和 `static_cast<int*>` 取得各自起始值。
- 已定位真实挂起原因：两个 Producer 各生产 10 个、Consumer 只消费 10 个，Consumer 退出后缓冲区最终填满，Producer 永久等待 `not_full`，`main` 卡在 `join`。这是生产和消费总量不匹配造成的永久等待，不是 mutex 本身形成的死锁。
- 已开始讲解 2 Producer + 2 Consumer，但停止在正式实践前，不记为完成或掌握。

上述 queue 与多 Producer 内容来自 2026-09-09 学习对话，没有保存为源码或运行结果，因此不支持 L3。`MASTERY.md` 仅把已有单槽代码、历史运行证据和本次解释所达到的 producer-consumer 记录更新到 L2。

## 待验证

- 不看答案把 2 Producer + 1 Consumer 的消费总数改为 20，编译运行并验证 20 次生产、20 次消费、最终 queue 为空且所有线程退出。
- 清理 queue 版本中遗留的单槽变量 `bool full` 和 `int data`；这两项尚未实际修正。
- 将 Producer 的数据范围改为容易区分来源的范围，例如 0–9 与 100–109；当前 `prod_id_1 = 1`、`prod_id_2 = 2` 配合 `i + id` 会产生大量重复值，尚未实际修正。
- 独立完成并验证 2 Producer + 2 Consumer；今天只开始讲解。
- 若要取消写死的循环次数，引入 `done` 或 Producer 完成计数，并验证结束协议；不能仅用 `buffer.empty()` 判断整个系统已经结束。
- read-write lock 的独立复现及互斥关系验证仍需完成；`LinuxCodeSrc/DAY7/src.cpp` 对未创建的 `tid4` 调用 join 的问题尚未修正。
- 后续分别定位已有 mutex、`mmap`、FIFO reader 和 Mini Shell 练习中的问题。

## 下一次测试

1. 不看笔记修正 2 Producer + 1 Consumer：让 Consumer 消费 20 个，清理单槽遗留变量，给两个 Producer 分配清晰且不重叠的数据范围；编译运行并核对生产/消费总数、最终 queue 状态和线程退出。
2. 口述原程序为什么会卡住，逐步说明等待中的 Producer、已退出的 Consumer 和 `main` 的状态，并区分正常 blocking、永久等待和 mutex 死锁。
3. 把程序变为 2 Producer + 2 Consumer，先设计总生产量和各 Consumer 消费量，再独立实现、运行和观察 mutex 如何保证 Consumer 不会同时操作 queue。
4. 完成定次数版本后，再设计 Producer 完成状态，让 Consumer 在“所有 Producer 已结束且 queue 为空”时退出。

## 下一步

先完成并验证 2 Producer + 1 Consumer 的数量修正，再进入 2 Producer + 2 Consumer 正式实践；未获得独立代码和运行结果前不把 producer-consumer 升到 L3，也不跳到 Socket。
