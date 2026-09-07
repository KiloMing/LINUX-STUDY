# CURRENT

最后更新：2026-09-07

## 当前阶段

Linux 系统编程收尾与并发基础验证。

## 仓库已验证

- DAY1–DAY6 保存了 Linux 基础、Git/Makefile、文件 I/O、目录操作、pipe、进程、`fork/exec/wait`、Mini Shell、FIFO、`mmap`、semaphore、mutex 的笔记或源码练习。
- “仓库已验证”只表示存在学习证据，不等于已经达到独立实现等级。

## 学习者自述

- 2026-09-07 主要时间投入嵌入式工程，Linux 学习时间较少；详见 [当日记录](daily/2026-09-07.md)。当前统一使用 C++ + pthread。
- condition variable 已回顾：`ready=true` 只改变条件；`pthread_cond_signal()` 通知并唤醒等待线程（存在等待者时）；被唤醒后仍须重新获得 mutex，`pthread_cond_wait()` 才返回。知道 signal/broadcast 的区别。
- 在 `if` vs `while` / 虚假唤醒变式中答错一次，仍需复测，不能记为已掌握。
- pthread rwlock 已入门：理解读+读可并发、读+写不可并发、写+写不可并发；认识 `pthread_rwlock_t`、`pthread_rwlock_init`、`pthread_rwlock_rdlock`、`pthread_rwlock_wrlock`、`pthread_rwlock_unlock`、`pthread_rwlock_destroy` 以及 `pthread_create` / `pthread_join`。
- 尚未完成 2 Reader + 1 Writer 独立实现，rwlock 仍为学习中/待验证，不能升到 L3。

以上回顾不构成新的独立代码与运行证据，本次不修改 `MASTERY.md` 等级。

## 待验证

- 不看资料解释并复现 condition variable 的 wait/signal 完整流程。
- 不看资料实现 2 个 reader + 1 个 writer 的 rwlock 基本结构。
- 后续分别定位已有 mutex、`mmap`、FIFO reader 和 Mini Shell 练习中的问题。

一次答错只生成后续变式测试，不直接判定为长期知识缺口。

## 下一次测试

1. 不看笔记，先做 condition variable 的 `while` / 虚假唤醒变式复测：口述 `pthread_cond_wait()` 从进入等待到返回的完整状态变化，写出由 mutex 保护、使用 `while` 检查条件的同步骨架，并解释为何不能用 `if`。
2. 再不看答案，使用 C++ + pthread 独立写出 2 Reader + 1 Writer rwlock 最小程序，编译运行并检查互斥关系。

## 下一步

上述测试通过后再进入 producer-consumer（生产者消费者）练习。未通过时回补并复测，不直接跳到 Socket。
