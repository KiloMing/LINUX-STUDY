# CURRENT

最后更新：2026-09-21

## 当前阶段

当前正式主线切到 **阶段 3：CMake/C++ 工程化**。阶段 2 Socket 的独立验收不取消，改为并行补证；2026-09-20 已推进到 poll 与 epoll LT，但 poll/epoll 的闭卷独立实现、3 客户端运行证据以及既有 TCP 文件传输健壮性验收仍保留。

2026-09-21 完成 CMake 与 ROS2 构建链的入门实践。CMake 侧已经能在提示下解释 `project()`、`add_executable()`、target、单一 `main()` 入口、多个可执行目标、`find_package()`、`target_link_libraries()`，并区分配置与真正编译：`cmake -S . -B build` 负责生成构建系统，`cmake --build build` 才执行编译。已实际遇到并定位未保存文件、CMake 语法分号、in-source build 污染源码目录等问题。

ROS2 侧已创建 `ament_cmake` package，并开始理解 workspace / package / node、`find_package(rclcpp REQUIRED)`、`ament_target_dependencies()`、`install(TARGETS ...)`、`ament_package()`、`colcon build`、`source install/setup.bash`、`ros2 pkg executables` 与 `ros2 run` 的关系。当前能让 ROS2 识别到 `demo_cpp_pkg`，并通过 `ros2 pkg executables demo_cpp_pkg` 看到已安装 executable；仍出现过 target 名称不一致导致的 `No executable found`。这部分属于 guided learning evidence，尚未完成闭卷独立复现，因此 ROS2 只记入门，不提前进入 topic/service/action。

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

先做 CMake/ROS2 构建链闭卷复现，不看今天的 CMakeLists：

1. 从空目录创建一个普通 CMake 工程，至少包含两个独立 executable；解释 `add_executable(target sources...)`、唯一 `main()` 入口以及“多个源文件组成一个程序”和“多个 target 生成多个程序”的区别。
2. 使用 out-of-source build：`cmake -S . -B build`、`cmake --build build`；故意执行一次 in-source build，说明为什么会出现 `CMakeFiles/`、`CMakeCache.txt`、`Makefile` 等中间文件以及如何清理。
3. 给一个 target 添加外部依赖，解释 `find_package()` 负责“找到包”，`target_link_libraries()` / `ament_target_dependencies()` 负责“把依赖交给 target”。
4. 从标准 ROS2 workspace 结构创建一个 `ament_cmake` package，写最小 rclcpp node，完成 `colcon build --packages-select ... → source install/setup.bash → ros2 pkg executables → ros2 run` 全链路。
5. 故意把 executable 名写错一次，根据 `ros2 pkg executables <pkg>` 定位 `No executable found`，确认自己能区分源码文件名、CMake target 名和 `ros2 run` 的 executable 名。

通过以上测试后，再进入 ROS2 topic 的 publisher/subscriber。Socket 侧继续保留 poll/epoll LT 的独立 3 客户端验收和 TCP 文件传输健壮性补证。

## 下一步

当前学习顺序调整为：**CMake 基础闭卷复现 → ROS2 标准 workspace/package 构建链 → Node 与 CLI → Topic publisher/subscriber → Service → Action → Parameter/Launch**。进入 Topic 前必须先独立跑通 package 构建、安装、source 和 `ros2 run`，避免把 ROS2 当成黑盒。

Socket 不再继续扩展高并发服务器专项；已有 select/poll/epoll、TCP 文件传输和短读写等内容作为通用 Linux/通信能力并行补齐证据，不删除原验收要求。

长期路线见 [ROADMAP.md](ROADMAP.md)，教学与评级遵循 [TEACHING_PROTOCOL.md](TEACHING_PROTOCOL.md) 和 [MASTERY.md](MASTERY.md)。
