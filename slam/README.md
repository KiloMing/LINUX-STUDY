# SLAM 与导航

## 用途

保存运动学、odometry、状态估计、建图、定位和 Nav2 实践，并让每个结论能够追溯到数据、配置或理论依据。

## 前置

ROS 2 通信与调试、TF2/URDF/RViz、运动学、可靠 odom、IMU/LiDAR 数据和时间/坐标一致性。

## 当前证据

2026-10-04 已完成 Chapter 7 / 7.2.1 SLAM Toolbox 在线建图，并用 `map_saver_cli` 保存 `test_map.pgm` 与 `test_map.yaml`。静态仓储场景下采用 20Hz LiDAR、低速弧线运动并避免碰撞后，建图稳定性明显改善。

2026-10-05 已继续到已有地图定位与 Nav2：7.4.1 AMCL 初始位姿发布和 7.4.2 TF 实时位姿查询完成，7.4.3 已查看 `/navigate_to_pose` Action 接口并开始写 C++ Client。当前断点是类型别名/GoalHandle/Client 成员定义，下一步补构造函数、Goal、回调和构建依赖后进行实际单点导航。

见 [10 月 5 日定位与导航记录](../daily/2026-10-05.md) 和 [10 月 4 日建图记录](../daily/2026-10-04.md)。当前证据为学习会话和截图；10 月 5 日源码、最新运行配置及地图原文件尚未同步，闭卷独立复现待完成，MASTERY 等级不变。

## 完成证据

在仿真与真机分别完成可复现的建图、定位和导航；能够沿传感器、TF、状态估计、地图、规划和控制链路定位失败。

## 新内容位置

建议按 `kinematics/`、`odometry/`、`state_estimation/`、`mapping/`、`navigation/` 建立目录。每次实验保留配置、数据说明、指标与复盘，不提交大体积生成数据。
