# Gazebo / ros2_control / SLAM 工程学习快照

源码归档日期：2026-10-02。来源：Ubuntu 24.04 ARM64 / ROS2 Jazzy / Gazebo Harmonic，实际目录 `/home/kiloming/my_ros/chapt6_ws/src/mybot_description`。该次归档保存实际源码（仅规范行尾空白），不带 build/install/log，未修改虚拟机运行工程。更早版本保留在 Git 历史，见 [10 月 1 日记录](../../daily/2026-10-01.md)。

**最后更新：2026-10-03。学习快照/当前课程进度：第六章 ros2_control 已收尾，已进入第七章 7.2.1；当前阻塞点为地图与实时 LaserScan 错位/重影。** [10 月 3 日完整记录](../../daily/2026-10-03.md) 区分已经修复的启动链路问题、当前诊断证据与尚未实施的惯量修复。下列“当前源码”和 2026-10-02 证据仍是上次实际归档内容；本次未直接登录 Ubuntu 工作区重新同步，不能据此断言 10 月 3 日运行源码已逐字归档。

> 进度补充（2026-10-06）：课程工作区已经继续完成 7.4.3 单点导航、7.4.4 路点导航和 7.5 巡检控制闭环，包括 `SpeechText`/`espeak-ng` 播报、camera bridge 与 `cv_bridge`/OpenCV 到点拍照。详情见 [10 月 6 日记录](../../daily/2026-10-06.md)。本目录仍是 10 月 2 日源码快照，10 月 6 日最终运行源码尚未同步，不能用本目录复现或代替当天工程。

## 2026-10-03 会话增量

- 完成 `joint_state_broadcaster`、`JointGroupEffortController` 与 `diff_drive_controller`，理解 effort/velocity、command/state interface、claimed/unclaimed。
- `sim_control` / `gz_sim_control` 已整合 Gazebo、`robot_state_publisher`、`/clock` 与 `/scan` bridge、spawn robot、`joint_state_broadcaster` 与 `diff_drive_controller` 自动激活。
- 删除重复 `/clock` bridge，修复 `slam_toolbox` 的 `Detected jump back in time`；启动图中必须只保留一个 `/clock` publisher 链路。
- 为 `scan_bridge` 补 `additional_env=gz_env`，使其与 Gazebo Server 使用相同 `GZ_IP`；Gazebo `/scan` 已能桥接到 ROS2。Gazebo 话题列表命令为 `gz topic -l`。
- 已安装并启动 `slam_toolbox`，进入 7.2.1；已检查 `map→odom→base_footprint→base_link→laser_link`、`/map`、`/scan` 和 RViz `Fixed Frame=map`。
- 当前地图与实时 LaserScan 明显错位/重影。原地旋转时 `odom→base_footprint` x/y 基本不变、yaw 平滑，`map→odom` 基本稳定，但尚未完成 Gazebo ground-truth 与 odom 的定量对比。

## 当前首要修复与验证

1. `wheel.urdf.xacro` 中轮轴为 Y，visual/collision 也旋转到 Y，但 `<xacro:cylinder_inertia>` 仍把 Z 当圆柱轴。下一步应改为 `Iyy=I轴`、`Ixx=Izz=I垂直轴`，重新构建并完全重启；当前归档没有提前修改该源码。
2. 分别执行 90° 原地旋转和固定距离直线运动，对比 Gazebo ground-truth pose 与 odom。转角比例异常时再校准 `wheel_separation`，距离比例异常时再校准 `wheel_radius`。
3. 已归档模型几何给出轮距 0.22 m、轮子半径 0.032 m，先保留；避免在没有定量证据时同时修改多个参数。惯量方向不一致是待修正的模型问题，尚未证实它就是扫描错位的根因。
4. 在静态、特征更丰富的环境中低速建图；RViz 与所有算法节点统一 `use_sim_time=true`。实时扫描与地图稳定重合后，再保存地图并继续 Navigation2。

## 2026-10-02 已归档源码

- IMU 已 include 并实例化；IMU 50 Hz、LiDAR 10 Hz、RGBD 30 Hz，保留传感器原有配置。
- world 含 Physics、UserCommands、SceneBroadcaster、Sensors（ogre2）、Imu 系统。
- `ros2_control.xacro` 对接 left_joint/right_joint；velocity command、position/velocity state。
- Gazebo 插件 parameters 正确嵌套并指向已安装 config/ros2_control.yaml；CMake 安装 config/worlds；旧 DiffDrive 已注释。
- 同日修复新增 sim_control.launch.py：从同一 Xacro 产生 robot_description，robot_state_publisher 发布描述，Gazebo create 从该 topic 创建机器人；另桥接 Gazebo→ROS `/clock`。运行依赖已补 launch/xacro/ros_gz 等。
- launch 含虚拟机特定软件渲染设置及 `--render-engine ogre`；world Sensors 的 ogre2 原样保留。默认 GUI 和 server 分别启动，可用 gui:=false 关闭 GUI。后台运行依赖有效桌面显示授权，不能把“关闭 GUI”当作传感器无需渲染环境。

## 2026-10-02 归档时重新检查

- 从真实工作区重新展开 second_robot.urdf.xacro，check_urdf 成功：10 links / 9 joints，包含 imu_link。
- parameters 在 Gazebo plugin 内且实际安装 YAML 可读。
- left_joint/right_joint 的控制声明与展开 URDF 的物理 joint 逐项匹配。
- 仓库源码与虚拟机文件规范行尾后的内容一致；仅归档，无功能改写。
- 本次未重新启动 Gazebo、未重新构建、未执行控制器激活或运动测试。

## 2026-10-02 修复任务留存的运行证据

这些是此前任务生成、本次检查后归档的输出，不冒充本次新运行结果：

- [节点](evidence/2026-10-02/nodes-default.txt)：/controller_manager、/gz_ros_control、/robot_state_publisher、/ros_gz_bridge。
- [接口](evidence/2026-10-02/interfaces-default.txt)：两轮 velocity command 为 available/unclaimed；position/velocity state 齐全。尚未占用，不代表差速控制器已激活。
- [Gazebo 话题](evidence/2026-10-02/topics.txt)：包含 IMU、scan、RGBD image/depth/points/camera_info。
- [IMU 样本](evidence/2026-10-02/default-_imu.txt)：z≈9.8，四元数接近单位旋转。
- [样本摘要](evidence/2026-10-02/sensor-samples.txt)：记录 IMU、scan、RGBD 图像/深度/点云样本字节数与 SHA256。大体积原始文本留在虚拟机 diagnostics/2026-10-02/final，不提交重复大数据。非零文件大小仅证明有样本，不能证明持续频率与质量。

## 2026-10-02 源码的复现入口

在安装依赖的 Jazzy 环境，从工作区运行：

```bash
source /opt/ros/jazzy/setup.bash
colcon build --packages-select mybot_description
source install/setup.bash
export GZ_IP=127.0.0.1
ros2 launch mybot_description sim_control.launch.py
```

在相同 ROS_DOMAIN_ID/GZ_PARTITION 环境另开终端检查：

```bash
source /opt/ros/jazzy/setup.bash
source install/setup.bash
ros2 node list
ros2 control list_hardware_interfaces
ros2 control list_controllers
ros2 topic echo /clock --once
gz topic -l
gz topic -e -t /imu -n 1
```

以上是 10 月 2 日归档 launch 的入口，该版本尚未自动配置 ROS 传感器桥接。10 月 3 日会话报告已加入 `/scan` bridge 和控制器自动激活，但最新运行源码尚未同步到此目录。Gazebo 有话题不等于 ROS 有同名 topic。Image Display 可在 Gazebo 内选择 /camera/image。手动文件 spawn 时，统一生成和加载 /tmp/second_robot.urdf，仍需发布机器人描述并检查时钟，避免加载旧 /tmp/mybot.urdf。

## 2026-10-02 保留的待验收项（历史断点）

控制器配置与激活、轮速命令/反馈/里程计、完整重启及持续传感器数据；轮子几何与惯性坐标系一致性；旧 display_robot 启动路径和显示工具运行依赖。保留模型原状，不在归档中偷偷修正未验收项。课程实验成功不等于独立掌握。

10 月 3 日课程已完成控制器收尾，当前优先事项以上方“当前首要修复与验证”为准；历史断点保留用于追溯。
