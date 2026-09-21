# CMake

## 用途

掌握进入 ROS 2 前必须具备的现代 C++ 工程组织与可重复构建能力。

## 前置

能用编译器和 Makefile 构建多文件 C/C++ 程序。

## 当前证据

DAY2 有 Makefile 基础。2026-09-21 已开始实际 CMake：在提示下完成 `add_executable`、多个 target、out-of-source build、`find_package`、target 依赖和 ROS2 CMake 练习，并定位 CMakeLists 未保存、错误分号、in-source build 污染目录等真实问题。当前按 guided L2 记录；尚未从空目录独立完成多 target + library 工程，不升 L3。

## 完成证据

从空目录建立多 target 工程，拆分库与可执行文件，正确传递 include/link 依赖，并独立定位一次配置或链接错误。

## 新内容位置

按练习建立子目录，每个目录保留 `CMakeLists.txt`、源码、构建命令和问题记录；构建输出不提交。


## 2026-09-21 最小规则

普通 CMake 练习优先使用：

```bash
cmake -S . -B build
cmake --build build
```

`cmake .` 是 in-source configure，不等于“直接编译 cpp”；它会把 `CMakeFiles/`、`CMakeCache.txt`、`Makefile` 等构建文件放进当前源码目录。正式练习统一使用独立 `build/`，构建输出不提交。
