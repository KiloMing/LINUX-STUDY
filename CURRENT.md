# CURRENT

最后更新：2026-09-08

## 当前阶段

Linux 系统编程收尾与并发基础验证；当前使用 C++ + pthread。已推进到单槽循环生产者—消费者，多槽有界队列处于概念入门。

## 仓库已验证

- DAY1–DAY6 保存 Linux 基础、Git/Makefile、文件 I/O、目录操作、pipe、进程、`fork/exec/wait`、Mini Shell、FIFO、`mmap`、semaphore、mutex 的笔记或源码练习。
- 今天学习事实见 [daily/2026-09-08.md](daily/2026-09-08.md)，对应源码与资料在 [LinuxCodeSrc](https://github.com/KiloMing/LinuxCodeSrc)。存在代码或资料不等于完成独立能力等级测试。

## 学习者自述

- 已做 pthread rwlock 的 2 Reader + 1 Writer 实验，讨论调度不确定性、读读并发及读写/写写互斥；已学习 destroy 与 unlock 的区别。
- 已学习区分 pthread_t、pthread_mutex_t、pthread_cond_t、pthread_rwlock_t、pthread_attr_t、sem_t 的职责，以及条件变量 init/wait/signal/broadcast/destroy。
- 单槽 mutex + full + not_empty + not_full 的循环生产/消费 0～9 已独立实现并运行，聊天中有本人代码与运行反馈；不将此扩大为整章掌握或闭卷延迟复现通过。
- 能说明 signal 仅通知等待线程，被唤醒后仍须重新竞争 mutex；Producer 可能先再次拿锁，因 full 为真进入 wait、释放 mutex，让 Consumer 有机会执行。
- 开始理解多槽 buffer / queue / FIFO，尚未独立实现多槽版本。

本次保留 `MASTERY.md` 等级；旧台账的 producer-consumer L0 尚未反映今天实践，当前事实以本文件及当日记录为准，等级待独立复测后更新。

## 待验证

- 昨天的 `if` / `while` / 虚假唤醒变式尚无完整闭卷复测通过证据。
- 单槽程序脱离答案的延迟复现与调度变式；rwlock 独立复现及互斥关系的验证。收尾发现 `DAY7/src.cpp` 对未创建的 tid4 调用 join，须先定位修正，不能记为验证通过。
- 多槽有界队列的独立实现、空满等待和 FIFO 验证。
- 后续分别定位已有 mutex、`mmap`、FIFO reader 和 Mini Shell 练习中的问题。

一次答错只生成后续变式测试，不直接判定为长期知识缺口。

## 下一次测试

1. 不看笔记解释 wait 的完整状态变化，写出 mutex + while 骨架；复测虚假唤醒和 signal 时无人等待的情况。
2. 从空文件复现单槽 0～9，解释 Producer 再次先拿锁为何不会覆盖未消费数据，并验证输出及退出。
3. 独立复现 2 Reader + 1 Writer，验证读读并发与读写互斥，不假定线程执行顺序。

## 下一步

复测后独立实现容量为 5 的 queue/FIFO 有界缓冲区，验证空等待、满等待及先进先出。未通过则回补，不将多槽版本标记已掌握，也不直接跳到 Socket。
