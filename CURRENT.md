# CURRENT

最后更新：2026-09-06

## 当前阶段

Linux 系统编程收尾与并发基础验证。

## 仓库已验证

- DAY1–DAY6 保存了 Linux 基础、Git/Makefile、文件 I/O、目录操作、pipe、进程、`fork/exec/wait`、Mini Shell、FIFO、`mmap`、semaphore、mutex 的笔记或源码练习。
- “仓库已验证”只表示存在学习证据，不等于已经达到独立实现等级。

## 学习者自述

- condition variable 已学习。
- pthread read-write lock 正在学习。

以上两项尚无仓库内独立复现证据，不能直接记为已掌握。

## 待验证

- 不看资料解释并复现 condition variable 的 wait/signal 完整流程。
- 不看资料实现 2 个 reader + 1 个 writer 的 rwlock 基本结构。
- 后续分别定位已有 mutex、`mmap`、FIFO reader 和 Mini Shell 练习中的问题。

一次答错只生成后续变式测试，不直接判定为长期知识缺口。

## 下一次测试

不看笔记，先口述 `pthread_cond_wait()` 从进入等待到返回的完整状态变化，再写出由 mutex 保护、使用 `while` 检查条件的同步骨架。

## 下一步

先验证 condition variable 与 rwlock，再进入生产者消费者练习。未通过时回补，不直接跳到 Socket。
