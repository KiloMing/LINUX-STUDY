# CURRENT

最后更新：2026-09-20

## 当前阶段

当前 I/O 多路复用已从 select 推进到 poll，并开始 epoll LT。2026-09-20 已整理一份学习版 poll 多客户端 Echo Server，能说明 `pollfd.fd/events/revents`、`fd=-1` 空槽、监听 fd 与连接 fd 的分工，以及 `poll()` 只报告就绪而不保证固定长度 TCP 消息已经完整到达；同时开始学习 `epoll_create1/epoll_ctl/epoll_wait`、ADD/MOD/DEL 和监听 fd / 连接 fd 的事件分流。今天仍无独立 3 客户端运行证据，也未闭卷独立完成 epoll LT Server，Socket 维持 L2。

此前并发学习的未完成验收仍保留：producer-consumer 为 L2；rwlock 2R2W 的基础独立验证来自学习者自述，源码和输出仍待补存。

## 仓库已验证

- `daily/2026-09-15.md` 记录最小 Echo Server 已实际跑通，以及当时 TCP 合并读取现象。
- 独立源码仓库 [KiloMing/LinuxCodeSrc](https://github.com/KiloMing/LinuxCodeSrc) 的提交 [`bebcefb`](https://github.com/KiloMing/LinuxCodeSrc/commit/bebcefb7c156a968e79fcfc4153c12d1cee39524) 含 `20260918/client_file.cpp`、`server_file.cpp`、115 字节的 `aaa.txt` 与相同内容的 `recv_aaa.txt`。
- 2026-09-19 源码仓库提交 [`e4da15c`](https://github.com/KiloMing/LinuxCodeSrc/commit/e4da15c4df4fea2fc0d4bc736973c1ae7b17f1ae) 新增 `20260919/select_server.cpp`，记录 Select Server 学习版实现；同一提交还把 9 月 18 日文件传输代码中若干 `send()` 返回值和接收累计变量改为 `ssize_t`。
- 2026-09-20 源码仓库新增 `20260920/poll_server.cpp` 与 `20260920/README.md`，记录 poll 多客户端主干以及 epoll LT 入门模型；均标记为 guided learning evidence，不作为独立 L3 证据。
- 本学习仓库只保存学习事实、理解、问题和复测计划；源代码继续放在 `LinuxCodeSrc`。
- “仓库已验证”只表示证据存在，不等于已经达到闭卷独立实现等级。

## 学习者自述与对话记录

- 2026-09-18 文本文件传输已经成功；客户端在服务端返回 `"OK"` 后分块发送，服务端按 `file_size` 保存为 `recv_` 前缀文件。
- 能解释 `server_sock` 用于监听、`client_sock` 用于具体连接；fork 后父进程关闭连接 fd、子进程关闭监听 fd，子进程完成后应关闭连接并退出。
- 理解 `open/read/send` 已经发送原始字节，TCP 不存在单独的文本/二进制发送模式；二进制正文长度取 `read()` 返回值，不能用 `strlen()`。
- 上述实现经过逐步提示和调试，尚不满足 L3 的独立复现要求。详见 [daily/2026-09-18.md](daily/2026-09-18.md)。
- 2026-09-19 已学习 select/bitmap/fd_set 并提交学习版 `select_server.cpp`。
- 2026-09-20 继续到 poll 并开始 epoll LT：能说明一个 `server_sock` 对应多个 `client_sock`，poll 的两次遍历分别用于扫描就绪事件与寻找空槽；开始理解 epoll 的注册模型、`epoll_ctl()` 和 `epoll_wait()`。尚无独立多客户端运行证据，详见 [当日记录](daily/2026-09-20.md)。

## 待验证

- 客户端先检查 `argc`，正确处理 `stoi` 异常和端口范围。
- `send()` 返回值使用 `ssize_t`；`send_all()` / `recv_all()` / 文件写入处理短读写和 `EINTR`。
- 客户端累计接收完整的 2 字节 ACK；服务端检查 `recv_file()` 返回值，子进程执行 `close(client_sock) + _exit()`。
- 空文件、大于 4096 字节文件、包含 `0x00` 的二进制文件逐字节一致；多客户端同时连接，子进程正常回收。
- 明确结构体序列化和 `uint64_t` 字节序，增加最终完成 ACK、校验和和失败文件清理。
- producer-consumer 闭卷独立复现、3P2C 最终运行证据及 rwlock 源码/输出仍待补齐。

## 下一次测试

先闭卷说明 poll 的 `fd/events/revents`、两次遍历的不同目的，以及 `server_sock → accept() → client_sock` 的一对多关系；再从空文件独立写出 poll 多客户端 Echo Server，验证至少 3 个客户端收发、断连清理和新连接接入。随后闭卷实现 epoll LT：`epoll_create1 → epoll_ctl(ADD) → epoll_wait → accept/read → DEL/close`，保留代码与输出。

既有 TCP 文件传输闭卷复现与健壮性验收继续保留：

1. 闭卷画出多进程文件传输时序图，说明每个 fd 在父进程、子进程和客户端中的职责与关闭时机。
2. 从空文件独立实现修正版客户端和服务端，先完成参数检查、`ssize_t`、完整 ACK、`recv_file()` 返回值和子进程退出。
3. 传输空文件、文本文件和含 `0x00` 的二进制文件，用 `cmp` 或 SHA-256 验证内容完全相同。
4. 同时启动两个客户端，检查父进程仍能继续 `accept()`，并观察子进程退出与回收。
5. 完成后再做显式序列化与最终 ACK 变式；不把直接发送本机结构体作为跨平台协议。

## 下一步

当前先完成 poll 与 epoll LT 的独立复现，再按 `非阻塞+epoll ET → UDP → 应用层协议/序列化 → 综合遥测/控制项目` 推进；select 的独立验收仍保留，具体验收见 [socket/README.md](socket/README.md)。TCP 文件传输闭卷复现与健壮性验收同步补齐，不因进入 select 而跳过；并发阶段遗留验收继续补证据。

长期路线见 [ROADMAP.md](ROADMAP.md)，教学与评级遵循 [TEACHING_PROTOCOL.md](TEACHING_PROTOCOL.md) 和 [MASTERY.md](MASTERY.md)。
