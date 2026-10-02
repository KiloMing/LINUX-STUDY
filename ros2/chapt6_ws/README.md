# 2026-10-02 Gazebo / ros2_control 源码与学习快照

来源：Ubuntu 24.04 ARM64 / ROS2 Jazzy / Gazebo Harmonic，实际目录 `/home/kiloming/my_ros/chapt6_ws/src/mybot_description`。沿用昨日归档位置，保存当前实际源码（仅规范行尾空白），不带 build/install/log，不修改虚拟机运行工程。昨日版本保留在 Git 历史，见 [10 月 1 日记录](../../daily/2026-10-01.md)。

**学习快照/当前课程进度：6.5.2 gz_ros2_control 排查中。** [完整今日问题-原因-解决办法](../../daily/2026-10-02.md) 区分对话断点与后续修复证据。

## 当前源码

- IMU 已 include 并实例化；IMU 50 Hz、LiDAR 10 Hz、RGBD 30 Hz，保留传感器原有配置。
- world 含 Physics、UserCommands、SceneBroadcaster、Sensors（ogre2）、Imu 系统。
- `ros2_control.xacro` 对接 left_joint/right_joint；velocity command、position/velocity state。
- Gazebo 插件 parameters 正确嵌套并指向已安装 config/ros2_control.yaml；CMake 安装 config/worlds；旧 DiffDrive 已注释。
- 同日修复新增 sim_control.launch.py：从同一 Xacro 产生 robot_description，robot_state_publisher 发布描述，Gazebo create 从该 topic 创建机器人；另桥接 Gazebo→ROS `/clock`。运行依赖已补 launch/xacro/ros_gz 等。
- launch 含虚拟机特定软件渲染设置及 `--render-engine ogre`；world Sensors 的 ogre2 原样保留。默认 GUI 和 server 分别启动，可用 gui:=false 关闭 GUI。后台运行依赖有效桌面显示授权，不能把“关闭 GUI”当作传感器无需渲染环境。

## 本次归档重新检查

- 从真实工作区重新展开 second_robot.urdf.xacro，check_urdf 成功：10 links / 9 joints，包含 imu_link。
- parameters 在 Gazebo plugin 内且实际安装 YAML 可读。
- left_joint/right_joint 的控制声明与展开 URDF 的物理 joint 逐项匹配。
- 仓库源码与虚拟机文件规范行尾后的内容一致；仅归档，无功能改写。
- 本次未重新启动 Gazebo、未重新构建、未执行控制器激活或运动测试。

## 同日修复任务留存的运行证据

这些是此前任务生成、本次检查后归档的输出，不冒充本次新运行结果：

- [节点](evidence/2026-10-02/nodes-default.txt)：/controller_manager、/gz_ros_control、/robot_state_publisher、/ros_gz_bridge。
- [接口](evidence/2026-10-02/interfaces-default.txt)：两轮 velocity command 为 available/unclaimed；position/velocity state 齐全。尚未占用，不代表差速控制器已激活。
- [Gazebo 话题](evidence/2026-10-02/topics.txt)：包含 IMU、scan、RGBD image/depth/points/camera_info。
- [IMU 样本](evidence/2026-10-02/default-_imu.txt)：z≈9.8，四元数接近单位旋转。
- [样本摘要](evidence/2026-10-02/sensor-samples.txt)：记录 IMU、scan、RGBD 图像/深度/点云样本字节数与 SHA256。大体积原始文本留在虚拟机 diagnostics/2026-10-02/final，不提交重复大数据。非零文件大小仅证明有样本，不能证明持续频率与质量。

## 复现入口（下次完整验收）

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

ROS 传感器桥接尚未由本 launch 自动配置；Gazebo 有话题不等于 ROS 有同名 topic。Image Display 可在 Gazebo 内选择 /camera/image。手动文件 spawn 时，统一生成和加载 /tmp/second_robot.urdf，仍需发布机器人描述并检查时钟，避免加载旧 /tmp/mybot.urdf。

## 保留的待验收项

控制器配置与激活、轮速命令/反馈/里程计、完整重启及持续传感器数据；轮子几何与惯性坐标系一致性；旧 display_robot 启动路径和显示工具运行依赖。保留模型原状，不在归档中偷偷修正未验收项。课程实验成功不等于独立掌握。
