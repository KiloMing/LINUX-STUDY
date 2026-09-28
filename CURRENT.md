# CURRENT

最后更新：2026-09-28

## 当前进度快照

主线为 **Turtle Patrol System 独立综合项目**，工作区 `~/my_ros/my_test`。按用户学习记录，第四章 Parameter/Parameter Client、Launch、namespace/remapping、双 turtlesim 隔离已跟随教学完成；不等于通过独立章节验收。Service / Parameter / Launch 保持 guided/半独立 L2，不因一次成功升 L3。

完整过程见 [今日记录](daily/2026-09-28.md)。

## 本次实际源码核验

Codex 通过 Parallels 读取 `/home/kiloming/my_ros/my_test/src` 两个包，未修改工程或重新运行 ROS2。

- `turtle_patrol_interface`：SetTarget.srv 为 float64 target_x/target_y → bool accepted/string message；接口生成配置已存在。
- `turtle_patrol`：最小 set_target Service、main/init-make_shared-spin-shutdown、目标成员与相对 turtle1/pose Subscriber 已存在。
- 合法分支已设置 has_target_=true；非法分支已填 Response 并提前返回，从源码看不会覆盖旧目标，运行变式仍待验证。
- 当前坐标范围为 [0,12]，需统一可达边界；Pose 回调只打印坐标，尚无速度 Publisher 或闭环。
- controller 的 CMake 已绑定 rclcpp、turtlesim、接口依赖；package.xml 只补了接口依赖，**仍缺 rclcpp 与 turtlesim**。
- 已有构建日志显示 turtle_patrol rc=0，接口包在该次未选中；不等同于本次干净重建。

## 学习记录与证据边界

- 用户描述最后截图持续输出 x≈5.54、y≈5.54、theta≈0.00；原图本次未重新核验，未重新启动节点。
- Parameter、Parameter Client、Launch 和双系统运行按今日学习记录保存，本次未重跑课程示例。
- main/service API、shared_ptr 创建、Response 状态迁移、CMake 拼写和包依赖曾出错；纠正后保留延迟复测。
- 节点没日志不等于没运行；Fast DDS SHM 警告未证明阻断主线，未记为彻底修复。

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

1. 5–10 分钟闭卷写最小 Service/main，解释 unique/shared/weak、accepted 与到点、成员生命周期、future 和 wait_for_service。
2. 补 package.xml 的 rclcpp/turtlesim，验证两包依赖与构建；测试合法→非法→合法请求的 Response 和旧目标保留，统一坐标范围。
3. 延迟复测 Parameter 拒绝/批量更新、Parameter Client 失败，以及 Launch 类型、namespace/remapping 与 endpoint 隔离。
4. 历史 Socket/TCP、并发、Qt 主线程、speech_thread、旧闭环草稿等验收继续保留；不因当前主线变化自动认定完成。

## 下一步

短复测和 manifest 补齐后，从 `geometry_msgs::msg::Twist` Publisher（相对 `turtle1/cmd_vel`）开始，接 Pose 误差计算、转向/前进、限幅/到点停止，再接 Parameter、Client、单系统/双 namespace Launch。独立实现、变式、故障定位和延迟复现完成后再评 L3。
