# ROS 2

## 用途

保存 ROS 2 通信、调试、TF2、URDF、RViz、仿真、ros2_control 和传感器集成练习。

## 前置

Socket 基础、现代 CMake 与必要的 C++ 能力；实际 Ubuntu/ROS 版本必须先记录到 `ENVIRONMENT.md`。

## 当前证据

2026-09-21 已在本地创建 `ament_cmake` C++ package，开始练习 `rclcpp` 依赖、`add_executable`、`ament_target_dependencies`、`install(TARGETS ...)`、`ament_package()`、`colcon build`、`source install/setup.bash`、`ros2 pkg executables` 和 `ros2 run`。当前 package 已可被 ROS2 发现，并能查询已安装 executable；曾因 target 名不一致出现 `No executable found`。这属于 guided 入门证据，ROS2 构建基础记 L1；尚无独立 node/topic 数据流，TF2/URDF/RViz 仍为 L0。

2026-09-23 已完成 `time_topic` 定时发布（学习者自述），并开始 Subscription。新增 Timer 句柄与创建、lambda 捕获、Executor 调度以及 CMake target 未定义问题的记录，见 [当日笔记](../daily/2026-09-23.md)。

2026-09-24 已在真实工作区完成 Timer Publisher → Topic → Subscription 联调：package 构建成功，`timer`/`subscription` executable 可查询并实际持续收发。随后用 C++ 接入 espeak-ng 完成 Topic 文本语音输出，并真实排查 `undefined reference`、普通 library 链接及 `target_link_libraries` signature 冲突。当前正把语音播放从 Subscription callback 解耦到 queue + `speech_thread`；线程版尚待最终运行验证。Topic 基础记 L2，详见 [当日笔记](../daily/2026-09-24.md)。

## 完成证据

能够独立建立 package 和节点数据流，解释输入/输出/参数/依赖，使用 ROS 2 命令定位问题，并完成 TF/URDF/RViz 的机器人模型实践。

## 新内容位置

按 workspace 或明确主题建立子目录。不要提交 `build/`、`install/`、`log/`；记录所用发行版与官方文档链接。


## 当前学习顺序

```text
Topic + speech worker 运行验证
  → topic publisher/subscriber 闭卷独立复现
  → ros2 topic CLI 调试
  → CMake/ROS2 构建链补证
  → service
  → action
  → parameter / launch
```

进入 topic 前先确保能独立解释并跑通“源码 → CMake target → install → source → ros2 run”链路，避免只会复制命令。
