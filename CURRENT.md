# CURRENT

最后更新：2026-09-27

## 当前进度快照

当前主线为 **ROS2 第四章 Service → Parameter**。今日已跟随教学完成 Python 人脸检测 Service 4.2.1～4.2.4，以及 C++ Patrol Service 4.3.1～4.3.3 的接口、Server、异步 Client 实战。

**视频进度：4.3.3 完成；明天从 4.4.1《参数声明设置》开始。** Service 为 guided L2，独立复现和章节验收仍待验证。4.1.2 此前跳过，Parameter 仅概念预告，尚未开始参数实战。

## 仓库已验证

- 本仓库已保存 [2026-09-27 每日学习快照](daily/2026-09-27.md)，含两条 Service 链路、关键命令、错误修正与明天接续点；本次只核验文档，未重新运行 ROS2。
- [9 月 25 日 CycleContarl 旧草稿](ros2/2026-09-25-cycle-contarl-UNVERIFIED.md) 继续保留 UNVERIFIED；本次没有取得最终 ROS2 源码或重新执行构建。
- 既有 Linux/Socket/Topic 证据见 [9 月 25 日记录](daily/2026-09-25.md) 及其前序每日记录；历史证据不等于本次独立复测。

## 学习者自述与对话记录

- 闭环内容已推进到 atan2、角度误差归一化、角度阈值、距离比例控制与最大速度限制；最终到点行为仍需源码与运行证据。
- status_interfaces 接口生成与查询、status_publisher 的 /sys_status 发布和 topic echo 已跑通（按学习记录）。
- hello_qt、sys_status_display 已跑通；对话文字记录 Qt 窗口显示主机、CPU、内存和网络状态。本次未重新核验历史截图。
- 已理解用 std::thread 跑 rclcpp::spin，主线程跑 app.exec；GUI 跨线程直接更新需改为主线程处理，不能将“能显示”当作线程安全证据。
- Python `/face_detect` 已完成发图、CvBridge 转换、face_recognition 检测、返回坐标并绘框；对话记录 1 张脸及约 0.202s，历史截图本次未重新核验。
- C++ `/patrol` 先经命令行调用验证移动，再由 Timer + async_send_request Client 成功收到 `target accepted`。接受目标不等于已到点。
- `.venv`/解释器、Python import、spin 调用顺序、C++ 拼写与 CMake 依赖经过排错；编译成功后的 IntelliSense 飘红需配置生成接口的 include root。SHM 警告当天未阻断主线，未确认修复。
- Topic、CMake 保持 L2；Service 从概念入门推进到 guided L2，尚无闭卷独立复现。

## 待验证

- 独立复现 FaceDetector 与 Patrol 的接口、Server、Client，保存最终源码、构建与运行输出；补测非法目标、服务缺席、请求失败、图片读取失败和空检测结果。

- 客户端先检查 `argc`，正确处理 `stoi` 异常和端口范围。
- `send()` 返回值使用 `ssize_t`；`send_all()` / `recv_all()` / 文件写入处理短读写和 `EINTR`。
- 客户端累计接收完整的 2 字节 ACK；服务端检查 `recv_file()` 返回值，子进程执行 `close(client_sock) + _exit()`。
- 空文件、大于 4096 字节文件、包含 `0x00` 的二进制文件逐字节一致；多客户端同时连接，子进程正常回收。
- 明确结构体序列化和 `uint64_t` 字节序，增加最终完成 ACK、校验和和失败文件清理。
- producer-consumer 闭卷独立复现、3P2C 最终运行证据及 rwlock 源码/输出仍待补齐。

## 当前问题/待解决

- 9 月 25 日旧草稿遗留核验（9 月 26 日已学习阈值和限幅，最终源码尚未核验）：将 `cmd_vel.linear.y = k_ang * error_ang` 修正为 `angular.z`；确认 `<cmath>`/`M_PI`；核对闭环 executable 的 CMake 依赖与安装。`target_ang_` 可局部化；旧草稿的 `max_speed_` 未实际限幅，今日对话代码已出现限幅与 angular.z，但最终工程仍需归档核验。

- A) cpp-httplib 头文件引用路径尚需在实际工程中验证。对话中的目录是 `include/cpp-httplib/httplib.h`；视频的 `include_directories(include)` 配套 `#include "cpp-httplib/httplib.h"`，当前 `<httplib.h>` 则要求搜索起点指向 `include/cpp-httplib`。
- B) 需要继续理解 `start_download` 的线程创建与 callback 生命周期；实际源码未核验，不能直接认定异步方式或引用安全。
- C) Timer Publisher → Topic → Subscription 已实际跑通；仍需用 `ros2 topic info/echo/hz` 保存 CLI 观察证据，并从空白独立复现一次。
- D) espeak-ng 已接入 Subscription 并完成语音输出；已解决普通 library 链接与 plain/keyword signature 冲突。当前 queue + `speech_thread` 解耦代码尚待实际运行验证。
- E) `ROS_DISTRO`、Ubuntu 版本和 arch 均待实际学习环境命令证据；[ENVIRONMENT.md](ENVIRONMENT.md) 保持待确认。

## 下一次测试

1. 闭卷复述 Python 人脸检测与 C++ Patrol 的请求/响应链路、一次请求与 Timer Client 的差别，以及 SUCCESS 的含义。
2. 从空文件独立复现最小 Patrol Server/Client，记录正常/非法目标、服务未启动和退出行为；课程从 4.4.1 参数声明设置接续。
3. 并行补存 status 三个包的源码、构建、接口查询、topic echo 与 GUI 证据，独立复现发布/订阅。
4. 核验闭环 angular.z、角度跨界、阈值、最大速度和到点停止；核验 GUI 主线程更新、shutdown/join 与窗口退出。
5. 保留 speech_thread 运行/退出、CMake 构建链、Socket/TCP 和 producer-consumer/rwlock 的独立验收；历史清单见 [9 月 25 日记录](daily/2026-09-25.md) 及此前 daily。

## 下一步

**明天从 4.4.1《参数声明设置》开始，以 TurtleController 的固定控制参数为背景。** 课程推进与独立能力验收分开记录，既有待验证项继续并行补证，不阻断本次已明确的续学入口。长期路线和评级标准保持不变。
