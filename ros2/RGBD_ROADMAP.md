# RGB-D 视觉 SLAM 应用路线

更新：2026-10-13。前置里程碑：学习者确认激光雷达 SLAM + Nav2 仿真已跑通，ROS2 第八章结束。第一阶段已有部分运行确认，其余任务仍为待实施/待验收，不代表全部运行通过。

10 月 11 日进展：Gazebo RGB/Depth 显示、PointCloud2 桥接、RViz 点云显示已由学习者确认。点云 frame_id 为 `second_robot/base_footprint/camera_sensor`，临时 static_transform_publisher 补齐 TF 后显示成功；下一步工程化 TF、同步最终相机/bridge/launch 并补齐传感器验收，再继续 RTAB-Map。本仓库尚未同步这些最终源码，详见 [今日记录](../daily/2026-10-11.md)。

10 月 12 日进展：RTAB-Map 已在 Gazebo 室内复杂场景实际运行，能显示 RGB 图像和 3D Map，但平移后的点云重影/条纹和空间错位未解决。确认建图节点使用轮式 `/diff_drive_controller/odom`，关闭视觉/ICP 里程计；三个相机消息 header 使用 `camera_optical_frame`。在虚拟机约 33%～41% 实时运行率下，当前 Ubuntu Xacro 请求 120 Hz、640×480，建议先验证单帧几何/光学 TF、CameraInfo/同步及 odom，单变量测试降为 15 Hz。场景更换不解决核心异常；**第一、第二阶段仍未完成验收**，见 [10 月 12 日排错记录](../daily/2026-10-12.md)。

10 月 13 日进展：在 Ubuntu 仿真工作区试运行 `rgbd_odometry` + RTAB-Map，使用 RViz MapCloud/MapGraph 可视化；`/rgbd_odom` 可发布，TF 从 `map→odom→base_footprint→base_link` 曾完整连通，但会间歇性缺失视觉 `odom→base_footprint`。前端日志 `Registration failed: Not enough inliers 0/20`、`quality=0`，无效 Odometry 一帧协方差对角线为 `9999`，因此 **有消息频率不代表有效跟踪**。RGB 接收频率约 8.67Hz，视觉里程计多段约 0.2～6Hz，频率还需与图像同步/特征质量分开对照。静止/直行精度与地图质量未通过，第一、二阶段仍待验收。详见 [10 月 13 日记录](../daily/2026-10-13.md)。仓库尚未同步当天运行脚本和最终相机 Xacro/YAML，本次仅更新学习文档。

目标：使用成熟的 RTAB-Map 与 Nav2 完成从仿真到 FET3588 实体移动机器人的系统集成。当前不以研究视觉 SLAM 算法为主线，已有 Linux/C++、ROS2、TF、差速控制与导航知识继续复用。

配套 [视觉巡航教学计划与实施流程](VISUAL_PATROL_TEACHING_PLAN.md) 将本路线细化为第九至十五章、24 个学习单元，并补充 A/B/C/D 巡航、到点拍照和失败处理。

## 六阶段与验收

| 顺序 | 阶段 | 学习与实施任务 | 通过条件/留档 |
|---|---|---|---|
| 1 | Gazebo RGB-D | 检查现有普通相机，增加 RGB-D 传感器及 ros_gz 桥接；核对光学坐标系、内参、时间戳、深度单位、RGB-D 对齐和 QoS | RGB、Depth、CameraInfo、点云稳定；RViz 图像/点云与场景一致，机器人移动时 TF 正常；保存模型、launch、桥接配置及短 rosbag |
| 2 | RTAB-Map 建图/定位 | 先复用仿真底盘 odom 验证 RGB-D 建图，再单独测试 rgbd_odometry；在有纹理场景中验证轨迹和回环；保存数据库与二维占据地图，再重新启动进入定位模式 | 能建图、保存、加载并重新定位；记录里程计来源、TF 发布者、数据库与地图对应关系，保存启动参数与日志 |
| 3 | Nav2 导航 | 接入二维地图和定位；先用官方规划器建立基线，再对比自定义 A*；将深度点云用于局部障碍观测，核对地面过滤、障碍高度和 footprint | 完成目标点导航；新增障碍时能停车/绕行/重规划；完整重启后可复现，保存成功与失败案例 |
| 4 | Gemini Pro 真机相机 | 在 Ubuntu 上先单独验证 USB、RGB/Depth、兼容驱动、CameraInfo、对齐和 TF；再缓慢移动相机做真实室内建图/定位 | 实际 RGB-D 数据稳定，RTAB-Map 能建立并加载真实地图；记录设备型号、驱动分支/版本、帧率和掉帧，不依赖实体底盘 |
| 5 | FET3588 部署 | 核实板卡型号、BSP、系统/架构与相机驱动依赖，再确定 ROS2 版本；迁移相机、RTAB-Map 与 Nav2 启动配置 | 记录 CPU、内存、帧率、延迟、温度与持续运行结果；重启后能启动相机/建图定位/导航节点，运动输出待下一阶段验收 |
| 6 | 实体底盘集成 | FET3588 与 STM32/ESP32 选择一种通信方案；完成编码器、轮速 PID、速度下发和 odom 回传；标定轮径/轮距、相机外参，再接入视觉导航 | 先通过直行/圆弧/旋转/停车/失联停车，再完成真机建图、重定位、目标导航与动态障碍实验；留下完整源码、配置、日志与复现说明 |

每个阶段通过后再推进。每周安排一个可验收结果，驱动兼容性与板卡系统尚未核实，不承诺固定完成日期。手动遥控建图先跑通；无人值守的自主探索建图需要额外的探索策略，不把 SLAM + Nav2 的接通写成自动探索已实现。

## 下一次学习：先修复视觉里程计有效跟踪，再验收 RGB-D 建图

1. 捕获 `/rtabmap/odom_info` 的 `lost/features/matches/inliers` 和 `/rgbd_odom` 的有效/无效协方差，确认是否始终为 `inliers=0`。
2. 分层检查 RGB、Depth、CameraInfo 同帧时间戳与分辨率、内参/深度有效值、相机 optical TF、`/rgbd_image` 同步频率。谨慎区分软件渲染导致的低墙钟频率和算法失配。
3. 以 `map→odom→base_footprint→base_link` 为目标验证单一 TF 发布者；禁止用临时静态 `odom→base_footprint` 假装修复动态里程计。
4. 在**静止正常且连续有效跟踪**之后再做慢速直行、缓慢转弯，分别计算视觉、轮式 odom 相对于起点的误差，并与 Gazebo 真值比较；持续保存 TF 图、位姿曲线及节点日志。
5. 前端可靠后，再验收 RTAB-Map 的三维/二维地图、回环和保存/加载定位；若只对地图表现做视觉优化，暂不把配置推进到 Nav2。保留轮式 odom 方案作对照，后续再探索融合。

## 数据接口与迁移边界

目标数据流：`RGB + Depth + CameraInfo → 同步输入 + odom → RTAB-Map → 地图/定位 → Nav2 → 速度命令 → 底盘`；深度点云另一路进入障碍观测。RTAB-Map 的 RGB-D、同步与里程计模块参考 [官方 ROS2 仓库](https://github.com/introlab/rtabmap_ros/tree/ros2)。

- 每条 TF 只指定一个发布者：RTAB-Map 负责视觉定位时，不同时让 AMCL 发布同一条 `map → odom`；测试视觉里程计时不与底盘重复发布 `odom → base_footprint/base_link`。
- 沿用实际工程的 `base_footprint/base_link` 层级；相机增加正确的安装与 optical frame，真机外参必须重新测量。
- 保存 RTAB-Map 数据库用于视觉定位，并保留供 Nav2 使用的二维栅格地图；三维点云文件不直接等同于二维导航地图。
- 切换真机后关闭仿真时间并检查时钟同步，逐项核对图像编码、深度单位、时间戳、QoS、相机内参及速度消息类型。
- Gazebo 传感器与 bridge 替换成真机驱动，仿真底盘替换成 MCU 通信与编码器反馈；尽量维持 ROS2 接口，参数和外参按实测重配。

## 遗留项与真机进入条件

A* 距障与膨胀代价、footprint、狭窄通道表现需要进一步优化。仿真视觉联调先保留官方规划器基线；自定义 A* 作为对比实验。速度指令时间戳/超时问题必须查清并验证停车行为后，才能进入实体运动联调。

Gemini Pro 当前仅有引用会话中的深度 USB 识别记录（`2bc5:0614`，480 Mbps）；RGB 和 ROS2 驱动未验证。按实际设备和厂商兼容列表选择驱动，不直接套用 Gemini 2 配置。FET3588 系统/BSP 和 ROS2 版本尚待确认，不预设镜像可直接复用，也不假设 SLAM 自动使用 NPU。

实体底盘须先验收失联停车、急停、限速及编码器方向/单位。单个前向深度相机有盲区；为真机记录视野、碰撞尺寸和停车距离，再开展受控的导航实验。

官方入口：[ros_gz](https://github.com/gazebosim/ros_gz)、[RTAB-Map ROS2](https://github.com/introlab/rtabmap_ros/tree/ros2)、[Nav2](https://docs.nav2.org/)。具体参数以实际安装版本为准。

进度入口：[CURRENT](../CURRENT.md)；排错与证据边界：[2026-10-11](../daily/2026-10-11.md)。
