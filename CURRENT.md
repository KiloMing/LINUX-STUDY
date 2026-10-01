# CURRENT

最后更新：2026-10-01

## 当前进度快照

当前推进第六章 URDF/Xacro 机器人部件建模，承接 6.2.3 附近的学习。按学习者自述、截图和实际源码，base、wheel、caster、camera、laser 双 link 已拆分总装，视觉与碰撞模型已在 RViz 显示。轮子为 continuous joint，传感器为 fixed；IMU 仅 include、未实例化。精确播放位置未确认。

见 [今日学习快照](daily/2026-10-01.md) 与 [源码及验证](ros2/chapt6_ws/README.md)。本次检查 camera collision 拼写、laser 两个 link 的同级结构，并修复雷达 material 错放在 link 下的问题。截图属于学习时的 RViz 证据，本次未重新启动 RViz。独立复现待验证，不上调 MASTERY。

## 仓库已验证（2026-09-29 TF 快照）

- 实际工程来自虚拟机 ~/my_ros/tf_test，静态 (5,3,0)/60°，动态 (2,3,2)/30°，动态 sendTransform 已存在。
- 两个发布器重新构建成功；tf2_echo 三组查询成功，包括 base_link → target_point = (2.598,-1.500,-2.000)/30°。
- CMake/package.xml 无重复 tf2_geometry_msgs；两个目标已配置依赖和安装。
- tf_listen 仍是未完成草稿，保存为 .incomplete，不参与编译；不能把 CLI 成功写成 C++ 查询节点完成。
- 源码快照不含 build/install/log；实际工作区 src 下本次未发现这些误生成目录。

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
- E) TF 学习环境已确认 Ubuntu 24.04.5 / aarch64 / Jazzy，见 [ENVIRONMENT.md](ENVIRONMENT.md)。

## 下一次测试

1. 闭卷解释 visual/collision/inertial；复现并定位 collision 拼写及嵌套错误。
2. 修改 camera 尺寸并同步几何与惯性；根据宏公式解释 w/h/d 的轴向映射。
3. 画出 base_footprint → base_link → 轮子/相机/雷达支柱 → 雷达的 TF 树，区分 fixed 与 continuous。
4. 保留 TF 独立验收：解释父子 frame、四元数、sendTransform、lookupTransform 参数顺序与时间。

## 下一步

以已完成的部件总装为起点继续课程；先核对轮子几何旋转与惯性坐标系的一致性、补齐运行依赖和启动入口，再进入动力学仿真。IMU 文件存在但尚未实例化。RViz 碰撞显示成功不等于物理仿真或差速控制完成。

5.3.3 C++ listener 在仓库中仍为未完成草稿，后续补齐 Buffer、TransformListener、timer、lookupTransform、try-catch、main 与 CMake，并对照 CLI 验证；没有新证据前不标记完成。Turtle Patrol、Service/Parameter/Launch 与历史独立验收继续保留。
