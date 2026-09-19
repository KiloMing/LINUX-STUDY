# Socket

## 用途

学习机器人主机、MCU、上位机和网络传感器需要的 TCP/UDP 基础，不扩展成高并发服务器专项。

## 前置

Linux 文件描述符、进程/线程、基本错误处理达到可独立练习的程度。

## 当前证据

- 2026-09-13 开始学习 TCP 特性、两个 fd、Socket 调用流程和 IPv4 地址配置，见 [当日记录](../daily/2026-09-13.md)。
- 2026-09-15 最小 Echo Server 已实际跑通，并观察到 TCP 多次发送可能被合并读取，见 [当日记录](../daily/2026-09-15.md)。
- 2026-09-18 在逐步教学下完成多进程 TCP 文本文件传输：`file_Info → "OK" → 文件正文`，理解父子进程关闭对应 fd、完整收发和二进制字节流，见 [当日记录](../daily/2026-09-18.md)。源码证据位于独立仓库 [LinuxCodeSrc `bebcefb`](https://github.com/KiloMing/LinuxCodeSrc/commit/bebcefb7c156a968e79fcfc4153c12d1cee39524)。
- 2026-09-19 已提交学习版 Select Echo Server [`e4da15c`](https://github.com/KiloMing/LinuxCodeSrc/commit/e4da15c4df4fea2fc0d4bc736973c1ae7b17f1ae)，主干包含 `FD_ZERO/FD_SET/FD_ISSET/FD_CLR`、集合复制、`maxfd + 1`、监听 fd 就绪后 `accept()`、连接 fd 加入集合以及 `read/write` Echo。当前仍在巩固 `server_sock / accept / client_sock / fd_set` 的定义关系，尚无独立 3 客户端运行证据。

当前为 L2：能解释模型并跟随示例完成实验。尚缺闭卷独立复现、短读写与异常路径修正、二进制哈希验证和协议变式，不升 L3。

## 课程与阶段路线

以《C++网络编程基础.docx》为主教材：TCP 基础、多进程与文件传输之后，衔接握手/挥手、缓冲区/Nagle，再进入 I/O 多路复用。文档存在章节编号重复，按标题与示例定位；`tcpselect.cpp`、`tcppoll.cpp`、`tcpepoll.cpp` 对应服务端主线。UDP 实战和应用层协议作为课程补充，不冒充原文已完整覆盖。

2026-09-19 当前点：TCP 基础已覆盖，正式进入 select；概念学习不代表独立实现，Socket 仍为 L2，见 [当日记录](../daily/2026-09-19.md)。

路线：**select → poll → epoll LT → 非阻塞+epoll ET → UDP → 应用层协议/序列化 → 综合遥测/控制项目**。

| 阶段 | 学习重点 | 验收 |
|---|---|---|
| N0 TCP 复盘与补验收 | 握手/挥手、缓冲区、Nagle、listen/accept、短读写 | 解释 send 成功的含义；补齐下方文件传输闭卷与健壮性验收 |
| N1 select（当前） | bitmap、fd_set、FD_ZERO/SET/CLR/ISSET、maxfd+1、timeout、监听/连接 fd | 独立写出多客户端 Echo Server，至少 3 个客户端同时收发，断连清理后继续接入 |
| N2 poll | pollfd、events/revents、与 select 的限制和扫描方式对照 | 将自己的 select Server 改为 poll Server 并复测 |
| N3 epoll LT | epoll_create1/ctl/wait、ADD/MOD/DEL、注册与就绪事件 | 独立实现 LT Echo Server，解释与 select/poll 的结构差异 |
| N4 非阻塞+epoll ET | O_NONBLOCK、EAGAIN/EWOULDBLOCK、LT/ET、循环 accept/recv、未发完数据管理 | 故意少读观察问题，再修复为处理到 EAGAIN；正确保留待发送数据 |
| N5 UDP | SOCK_DGRAM、sendto/recvfrom、数据报边界与场景取舍 | 独立实现 UDP client/server，与 TCP 比较 |
| N6 应用层协议/序列化 | 长度前缀、字节序、短读写、完整 ACK、断线处理 | 重构文件协议，显式序列化，完成最终 ACK 与校验变式 |
| N7 综合遥测/控制项目 | 多客户端、TCP/UDP 取舍、超时、日志和异常处理 | 留下可重复运行的小型通信项目，并用 GDB、strace、ss 和日志定位故障 |

按约 8–12 个学习单元滚动安排，按证据推进，不固定为日历天数。不扩展线程池、Reactor 框架、百万连接优化等高并发服务器专项；完成后衔接 CMake/C++ 工程化，长期 ROS/SLAM 路线不变。

## 当前 select 练习

按“为什么需要多路复用 → fd_set/bitmap → 五个参数 → 监听 fd 就绪后 accept → 加入连接 fd → 连接 fd 就绪后 recv → 断开后 close + FD_CLR → 多客户端测试 → 分析限制 → 闭卷重写”推进。每轮复制长期集合，正确处理超时与错误；检查 FD_SETSIZE 范围，不能把就绪当成完整消息到达，也不能假设一次 send 发完。select 可用于客户端，当前教学优先服务端。

## 保留的文件传输验收

1. 独立复现多进程文件传输，补齐参数检查、`ssize_t`、完整 ACK、短写、`EINTR` 和子进程退出。
2. 传输空文件、大文件和含 `0x00` 的文件，用 `cmp`/SHA-256 验证。
3. 同时连接两个客户端并处理子进程回收。
4. 将本机结构体改成显式序列化，增加最终完成 ACK 与校验。
5. 上述项目随新主线继续补验收，不因进入 select 而视为完成；协议变式在 N6 汇总验证。

## 完成证据

独立完成 select 多客户端 Echo Server、既有 TCP 文件传输和 UDP client/server；能实现并比较 poll、epoll LT 与非阻塞 ET，处理短读写、断连和端口错误，完成模拟遥测或控制项目。

## 新内容位置

按 `tcp/`、`udp/`、`robot_protocol/` 等真实练习建立子目录，每个练习同时保存运行说明。学习日报放在 `daily/`，源代码仍放独立的 `LinuxCodeSrc` 仓库。
