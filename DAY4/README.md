# Linux 学习日志｜2026-08-26（更新版）

## 一、今日学习主题

今天继续推进 **Linux 系统编程阶段 3：进程管理**，重点完成并串联：

- `wait()` / `waitpid()`
- 僵尸进程与孤儿进程
- `exec` 系列函数
- `fork() + exec() + wait()`
- Shell 输入重定向 `<`
- Mini Shell 的基本执行模型
- 使用 C++ 的 `std::string`、`std::vector`、`std::istringstream` 解析命令
- 将 C++ 参数转换为 `execvp()` 所需的 `char *argv[]`

---

# 二、`wait()`：等待并回收子进程

## 1. `wait()` 是什么

```c
wait(NULL);
```

可以理解为：

```text
等待某个子进程结束
+
回收子进程留下的退出信息
```

注意：

```text
wait() ≠ 杀死子进程
```

子进程自己结束，父进程负责等待并回收。

## 2. `wait()` 在哪个进程执行

通常由父进程执行：

```c
pid_t pid = fork();

if (pid == 0)
{
    exit(0);      // 子进程
}
else
{
    wait(NULL);   // 父进程
}
```

`wait()` 的本质是：**当前进程等待自己的子进程。**

## 3. `wait()` 的返回值

如果子进程：

```c
exit(5);
```

并不意味着 `wait()` 返回 `5`。

```c
int status;
pid_t ret = wait(&status);
```

其中：

```text
ret
→ 被回收的子进程 PID
```

退出码通过：

```c
WEXITSTATUS(status)
```

获得。

```c
if (WIFEXITED(status))
{
    printf("exit code = %d\n", WEXITSTATUS(status));
}
```

若子进程执行 `exit(5)`：

```text
wait() 返回值       → 子进程 PID
WEXITSTATUS(status) → 5
```

---

# 三、`waitpid()`

```c
waitpid(pid, &status, 0);
```

含义：

```text
pid     → 指定等待哪个子进程
&status → 保存退出状态
0       → 阻塞等待
```

如果指定子进程还没结束，父进程会阻塞；子进程结束后，`waitpid()` 回收它并返回。

## `WNOHANG`

```c
waitpid(pid, &status, WNOHANG);
```

表示非阻塞检查：

```text
子进程未结束 → 返回 0
成功回收     → 返回子进程 PID
出错         → 返回 -1
```

---

# 四、僵尸进程与孤儿进程

## 1. 僵尸进程 Zombie

产生条件：

```text
子进程已经结束
+
父进程还活着
+
父进程尚未回收子进程
```

僵尸进程已经不运行代码，只是内核仍保存少量退出信息，等待父进程使用：

```c
wait();
waitpid();
```

回收。

## 2. 孤儿进程 Orphan

产生条件：

```text
父进程先结束
+
子进程还在运行
```

孤儿进程不会自动死亡，会由系统中的其他进程接管。

## 3. 核心区别

```text
Zombie：子先结束，父没回收
Orphan：父先结束，子还活着
```

---

# 五、为什么普通 `pipe()` 示例常常没写 `wait()`

很多教学示例为了突出 `pipe()`、`fork()`、`read/write`，会省略 `wait()`。

短生命周期示例中父子进程都很快退出，即使短暂出现 Zombie，也不明显。

但长期运行程序，例如多进程服务器，如果不断 `fork()` 却不回收子进程，就可能积累大量 Zombie。

因此：**长期运行的父进程必须考虑子进程回收。**

---

# 六、`exec()` 系列函数

## 1. `fork()` 与 `exec()` 的区别

```text
fork()
→ 创建新进程

exec()
→ 不创建新进程
→ 用新的程序替换当前进程中的程序映像
```

例如：

```c
execlp("ls", "ls", "-l", NULL);
```

等价于执行：

```bash
ls -l
```

## 2. `exec()` 成功后不会返回

```c
printf("A\n");
execlp("echo", "echo", "hello", NULL);
printf("B\n");
```

如果 `exec` 成功：

```text
打印 A
↓
当前程序被 echo 替换
↓
执行 echo hello
↓
进程结束
```

因此 `B` 不会打印。

结论：

```text
exec 成功 → 不返回
exec 失败 → 返回 -1，继续执行后面的错误处理代码
```

规范写法：

```c
execlp("ls", "ls", "-l", NULL);
perror("execlp");
_exit(1);
```

---

# 七、`execlp()` 参数解析

```c
execlp("ls", "ls", "-l", NULL);
```

拆开：

```text
第一个 "ls" → 要执行哪个程序
第二个 "ls" → argv[0]
"-l"          → argv[1]
NULL          → 参数结束
```

最终等价于：

```bash
ls -l
```

---

# 八、`l / v / p` 的含义

```text
l = list   → 参数一个一个写
v = vector → 参数通过数组传入
p = PATH   → 根据 PATH 搜索程序
```

常见四个函数：

| 函数 | 参数方式 | 是否搜索 PATH |
|---|---|---|
| `execl` | 一个一个写 | 否 |
| `execlp` | 一个一个写 | 是 |
| `execv` | 数组 | 否 |
| `execvp` | 数组 | 是 |

例如：

```c
char *args[] = {"gcc", "main.c", "-o", "main", NULL};
execvp("gcc", args);
```

等价于：

```bash
gcc main.c -o main
```

---

# 九、`fork() + exec() + wait()`

典型结构：

```c
pid_t pid = fork();

if (pid == 0)
{
    execlp("ls", "ls", "-l", NULL);
    perror("execlp");
    _exit(1);
}
else
{
    wait(NULL);
    printf("Child process finished\n");
}
```

执行流程：

```text
             fork()
            /      \
         父进程    子进程
           ↓         ↓
        wait()     exec()
           ↓         ↓
          阻塞       ls
                     ↓
                    结束
                     ↓
        wait 回收 ←──┘
           ↓
        父进程继续
```

这就是 Shell 执行外部命令的基础模型。

---

# 十、Shell 输入重定向 `<`

命令：

```bash
./main < kjkk
```

不是执行 `kjkk`，而是：

```text
kjkk 文件
↓
stdin（fd 0）
↓
./main
```

因此 `stdin` 并没有写死为键盘。

默认：

```text
键盘 → stdin(fd 0) → 程序
```

重定向后：

```text
文件 → stdin(fd 0) → 程序
```

`<`、`|`、`dup2()` 都可以改变标准输入/输出的连接关系。

---

# 十一、为什么 `execlp(command, command, NULL)` 不能直接执行 `ls -l`

如果用户输入：

```text
ls -l
```

则：

```cpp
command == "ls -l";
```

直接：

```c
execlp(command, command, NULL);
```

相当于：

```c
execlp("ls -l", "ls -l", NULL);
```

系统会寻找一个真正名叫 `ls -l` 的程序。

但正确结构应该是：

```text
程序名：ls
参数：-l
```

因此 Shell 必须先把命令字符串拆成参数数组，再交给 `execvp()`。

---

# 十二、使用 C++ 解析命令

为了减少 C 字符串处理的复杂度，当前 Mini Shell 外围逻辑改用 C++：

```text
C++：string / vector / getline / istringstream
POSIX：fork / execvp / waitpid
```

## 1. `getline()`

```cpp
std::string command;
std::getline(std::cin, command);
```

输入：

```text
ls -l /tmp
```

得到：

```cpp
command == "ls -l /tmp";
```

## 2. `istringstream`

```cpp
std::istringstream iss(command);
```

可以理解为：**把一个字符串变成一个可以像 `cin` 一样使用 `>>` 读取的输入流。**

```cpp
std::string token;
while (iss >> token)
{
    ...
}
```

如果：

```text
command = "ls -l /tmp"
```

则依次得到：

```text
"ls"
"-l"
"/tmp"
```

## 3. 保存到 `vector<string>`

```cpp
std::vector<std::string> tokens;
std::string token;

while (iss >> token)
{
    tokens.push_back(token);
}
```

输入：

```text
gcc main.c -o main
```

最终：

```text
tokens[0] = "gcc"
tokens[1] = "main.c"
tokens[2] = "-o"
tokens[3] = "main"
```

---

# 十三、当前 Mini Shell 核心代码

```cpp
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <string>
#include <sstream>

int main(void)
{
    std::string command = "";
    std::getline(std::cin, command);

    std::istringstream iss(command);
    std::vector<std::string> tokens;
    std::string token;

    while (iss >> token)
    {
        tokens.push_back(token);
    }

    if (tokens.empty())
    {
        return 0;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        std::cerr << "Fork failed" << std::endl;
        return 1;
    }
    else if (pid == 0)
    {
        std::vector<char *> args;

        for (auto& t : tokens)
        {
            args.push_back(t.data());
        }

        args.push_back(nullptr);

        execvp(args[0], args.data());

        std::cerr << "Exec failed" << std::endl;
        _exit(1);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
    }

    return 0;
}
```

---

# 十四、为什么要 `vector<string>` → `vector<char*>`

当前：

```cpp
std::vector<std::string> tokens;
```

例如：

```text
tokens[0] = "ls"
tokens[1] = "-l"
tokens[2] = "/tmp"
```

但 `execvp()` 是 POSIX C 接口，需要类似：

```c
char *argv[]
```

因此：

```cpp
std::vector<char *> args;

for (auto& t : tokens)
{
    args.push_back(t.data());
}

args.push_back(nullptr);
```

得到：

```text
args[0] → "ls"
args[1] → "-l"
args[2] → "/tmp"
args[3] → nullptr
```

`nullptr` 用来告诉 `execvp()`：**参数到这里结束。**

---

# 十五、`args.data()`

对于：

```cpp
std::vector<char *> args;
```

```cpp
args.data()
```

表示取得 `vector` 底层连续数组的首地址。

因此：

```cpp
execvp(args[0], args.data());
```

可以理解成：

```text
args[0]
→ 要执行哪个程序

args.data()
→ 把整个参数数组交给 execvp()
```

例如：

```text
args[0] = "ls"
args[1] = "-l"
args[2] = "/tmp"
args[3] = nullptr
```

最终执行：

```bash
ls -l /tmp
```

---

# 十六、当前代码需要注意的问题

## 1. 空命令

如果用户直接按回车，`tokens` 为空，因此应在 `fork()` 前判断：

```cpp
if (tokens.empty())
{
    return 0;
}
```

## 2. 未使用的头文件

如果没有使用 `stat()`，则：

```cpp
#include <sys/stat.h>
```

可以删除。

## 3. 编译方式

当前代码是 C++：

```bash
g++ main.cpp -o main
```

---

# 十七、今日核心知识链

```text
用户输入命令
↓
getline()
↓
完整字符串
↓
istringstream
↓
按空白拆分
↓
vector<string> tokens
↓
转换成 vector<char*> args
↓
fork()
↓
┌──────────────────────────┐
│                          │
父进程                    子进程
│                          │
waitpid()              execvp()
│                          │
阻塞                    执行命令
│                          │
│                       命令结束
│                          │
waitpid 回收 ←─────────────┘
│
父进程继续
```

---

# 十八、今日学习进度

```text
阶段 3：Linux 系统编程

3.1 程序执行基础                  ✅
3.2 文件 I/O                      ✅
3.3 目录操作                      ✅

3.4 进程                          🟡
├── PID / PPID                    ✅
├── fork()                        ✅
├── 多次 fork()                   ✅
├── 父子进程内存关系              ✅
├── exit() / _exit()              ✅
├── wait()                        ✅
├── waitpid()                     ✅ 基础
├── WNOHANG                       ✅ 基础
├── Zombie / Orphan               ✅
├── exec 基本原理                 ✅
├── execl / execlp                ✅
├── execv / execvp                ✅
├── fork + exec + wait            ✅
└── Mini Shell                    🟡
    ├── 读取整行命令              ✅
    ├── istringstream 命令拆分     🟡
    ├── vector<string> 参数保存    🟡
    ├── 转换为 char* argv[]        🟡
    └── execvp 执行动态命令        🟡
```

---

# 十九、下一次学习起点

下一次从当前 Mini Shell 继续，不跳步：

```text
1. 继续弄懂 istringstream
2. 弄懂 vector<string> → vector<char*>
3. 弄懂 args.data()
4. 给 Shell 加 while 循环，使其可连续执行命令
5. 处理 exit 内建命令
6. 后续再逐步加入重定向和 pipe
```

当前优先目标是彻底掌握：

```text
输入 → 解析 → fork → execvp → waitpid
```
