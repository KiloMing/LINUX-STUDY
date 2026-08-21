nux 学习记录 - 2026-08-21

## 今日学习目标

- 理解 Linux 文件系统基本操作
- 掌握 Linux 用户、用户组、权限管理
- 掌握文件所有者与权限修改
- 熟悉 vim 基础编辑操作
- 理解 Linux 输入输出重定向
- 学习 Git 与 Linux 开发环境结合

---

# 一、Linux 基础环境认识

## 1. Mac 是否可以当 Linux 使用？

### 结论

Mac 不能直接等同于 Linux，但是可以作为 Linux 开发环境。

原因：

- macOS 和 Linux 都属于 Unix-like 系统
- 两者拥有相似的终端命令
- 都支持：
  - bash/zsh
    - ssh
      - gcc/g++
        - vim
          - git

          区别：

          | 项目 | Linux | macOS |
          |---|---|---|
          | 内核 | Linux Kernel | Darwin/XNU |
          | 包管理 | apt/yum/pacman | brew |
          | 开发环境 | 原生 Linux | Unix环境 |

          如果学习：

          - Linux命令
          - C/C++开发
          - Git
          - 服务器操作

          Mac 完全可以使用。


          ---

          # 二、Linux 文件权限系统

          ## 1. 查看文件权限

          命令：

          ```bash
          ls -l
