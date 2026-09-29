# 第五章 TF C++ 学习快照（2026-09-29）

来自虚拟机 `~/my_ros/tf_test`，环境 Ubuntu 24.04.5 / aarch64 / ROS 2 Jazzy。详细错误、进度与证据见 [当日记录](../../daily/2026-09-29.md)。

- `src/demo_cpp_tf/src/tf_static.cpp`：map → target_point，(5,3,0)，yaw=60°。
- `src/demo_cpp_tf/src/tf_dynamic.cpp`：每 10ms 发布 map → base_link，(2,3,2)，yaw=30°；位置保持常量。
- [未完成查询草稿](drafts/tf_listen.cpp.incomplete)：原始内容保存，不参与编译，明天继续 5.3.3。
- CMake 与 package.xml 已含两个发布器需要的依赖，tf2_geometry_msgs 无重复；CMake 安装两个目标到 lib/demo_cpp_tf。只新增草稿说明，不添加未实现目标。

在 Linux / ROS 2 Jazzy 环境，从本目录（workspace 根）执行：

```bash
source /opt/ros/jazzy/setup.bash
colcon build --packages-select demo_cpp_tf
source install/setup.bash
ros2 run demo_cpp_tf tf_static
```

另一个已 source 相同环境的终端运行：

```bash
ros2 run demo_cpp_tf tf_dynamic
```

保持发布器运行，在第三个相同环境终端查询：

```bash
ros2 run tf2_ros tf2_echo map target_point
ros2 run tf2_ros tf2_echo map base_link
ros2 run tf2_ros tf2_echo base_link target_point
```

最后一项实测平移约 (2.598,-1.500,-2.000)，yaw=30°。每个 echo 持续运行，Ctrl+C 后执行下一项。不要在 src 下构建；build/install/log 由根 .gitignore 忽略，不属于源码快照。
