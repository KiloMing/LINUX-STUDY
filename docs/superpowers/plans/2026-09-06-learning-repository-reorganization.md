# Linux-to-Robotics Learning Repository Reorganization Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Reorganize `LINUX-STUDY` into an evidence-based Linux-to-robotics learning repository while preserving all original notes and source exercises.

**Architecture:** Keep `DAY1/` through `DAY6/` as immutable historical paths and add a root control layer plus topic indexes for future work. A dependency-free Python validator protects the required structure, evidence labels, environment placeholders, local links, legacy files, and absence of tracked build artifacts.

**Tech Stack:** Markdown, Git, Python 3 standard library (`unittest`, `pathlib`, `subprocess`, `re`)

**Spec:** `docs/superpowers/specs/2026-09-06-learning-repository-reorganization-design.md`

## Global Constraints

- Base all progress claims on repository evidence at `1dc2bde` or label them as learner-reported and awaiting verification.
- Preserve `DAY1/` through `DAY6/`, their source files, small text fixtures, and original note style.
- Do not rewrite Git history, force-push, mass-rename legacy paths, or silently fix teaching exercises.
- Record unknown operating-system, toolchain, ROS, and hardware values as `待确认`.
- Remove only the explicitly audited compiled ELF, `.o`, and `.d` files listed in Task 3.
- Keep all work on `codex/reorganize-learning-repo` unless the user later authorizes push or merge.

---

### Task 1: Add the repository validator with TDD

**Files:**
- Create: `tests/test_validate_repository.py`
- Create: `scripts/__init__.py`
- Create: `scripts/validate_repository.py`
- Test: `tests/test_validate_repository.py`

**Interfaces:**
- Consumes: Python 3 standard library and the future module `scripts.validate_repository`.
- Produces: executable contract tests, `validate_repository(repo: Path) -> list[str]`, and CLI exit code 0 for valid repositories or 1 for invalid repositories.

- [ ] **Step 1: Create failing tests for the repository contract**

```python
import subprocess
import tempfile
import unittest
from pathlib import Path

from scripts.validate_repository import validate_repository


ROOT_FILES = (
    "README.md",
    "ROADMAP.md",
    "CURRENT.md",
    "MASTERY.md",
    "ENVIRONMENT.md",
    "TEACHING_PROTOCOL.md",
)
TOPIC_DIRS = ("daily", "linux", "socket", "cmake", "ros2", "slam", "projects")
LEGACY_FILES = (
    "DAY1/README.md",
    "DAY2/README.md",
    "DAY3/README.md",
    "DAY4/README.md",
    "DAY5/20260827.md",
    "DAY6/readme.md",
)


class RepositoryFixture:
    def __init__(self, root: Path) -> None:
        self.root = root
        subprocess.run(["git", "init", "-q"], cwd=root, check=True)
        for relative in ROOT_FILES + LEGACY_FILES:
            path = root / relative
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text("# Document\n", encoding="utf-8")
        for directory in TOPIC_DIRS:
            path = root / directory / "README.md"
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text("# Index\n", encoding="utf-8")
        (root / "CURRENT.md").write_text(
            "# Current\n仓库已验证\n学习者自述\n待验证\n下一次测试\n下一步\n",
            encoding="utf-8",
        )
        (root / "ENVIRONMENT.md").write_text(
            "# Environment\n操作系统：待确认\n编译器：待确认\nCMake：待确认\n"
            "ROS 2：待确认\n开发板：待确认\n传感器：待确认\n",
            encoding="utf-8",
        )
        subprocess.run(["git", "add", "."], cwd=root, check=True)


class ValidateRepositoryTests(unittest.TestCase):
    def test_complete_contract_has_no_errors(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            fixture = RepositoryFixture(Path(directory))
            self.assertEqual(validate_repository(fixture.root), [])

    def test_missing_required_file_is_reported(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            fixture = RepositoryFixture(Path(directory))
            (fixture.root / "ROADMAP.md").unlink()
            self.assertIn("missing required file: ROADMAP.md", validate_repository(fixture.root))

    def test_broken_local_markdown_link_is_reported(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            fixture = RepositoryFixture(Path(directory))
            (fixture.root / "README.md").write_text(
                "# Document\n[missing](linux/not-here.md)\n", encoding="utf-8"
            )
            errors = validate_repository(fixture.root)
            self.assertIn("broken local link: README.md -> linux/not-here.md", errors)

    def test_link_examples_inside_fenced_code_are_ignored(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            fixture = RepositoryFixture(Path(directory))
            (fixture.root / "README.md").write_text(
                "# Document\n" + "`" * 3
                + "markdown\n[example](not-a-real-file.md)\n"
                + "`" * 3 + "\n",
                encoding="utf-8",
            )
            self.assertEqual(validate_repository(fixture.root), [])

    def test_tracked_elf_is_reported(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            fixture = RepositoryFixture(Path(directory))
            artifact = fixture.root / "linux" / "demo"
            artifact.write_bytes(b"\x7fELFgenerated")
            subprocess.run(["git", "add", "linux/demo"], cwd=fixture.root, check=True)
            self.assertIn("tracked build artifact: linux/demo", validate_repository(fixture.root))


if __name__ == "__main__":
    unittest.main()
```

- [ ] **Step 2: Run the tests and confirm the expected import failure**

Run: `python3 -m unittest tests/test_validate_repository.py -v`

Expected: FAIL with `ModuleNotFoundError: No module named 'scripts.validate_repository'`.

- [ ] **Step 3: Implement the minimum validator needed by the tests**

```python
from __future__ import annotations

import re
import subprocess
import sys
from pathlib import Path


REQUIRED_FILES = (
    "README.md",
    "ROADMAP.md",
    "CURRENT.md",
    "MASTERY.md",
    "ENVIRONMENT.md",
    "TEACHING_PROTOCOL.md",
)
REQUIRED_DIRS = ("daily", "linux", "socket", "cmake", "ros2", "slam", "projects")
LEGACY_FILES = (
    "DAY1/README.md",
    "DAY2/README.md",
    "DAY3/README.md",
    "DAY4/README.md",
    "DAY5/20260827.md",
    "DAY6/readme.md",
)
CURRENT_MARKERS = ("仓库已验证", "学习者自述", "待验证", "下一次测试", "下一步")
ENVIRONMENT_FIELDS = ("操作系统", "编译器", "CMake", "ROS 2", "开发板", "传感器")
LOCAL_LINK = re.compile(r"\[[^\]]+\]\((?!https?://|mailto:|#)([^)]+)\)")
FENCED_CODE = re.compile(r"```.*?```", re.DOTALL)


def tracked_files(repo: Path) -> list[Path]:
    result = subprocess.run(
        ["git", "ls-files", "-z"], cwd=repo, check=True, capture_output=True
    )
    return [repo / item.decode() for item in result.stdout.split(b"\0") if item]


def validate_repository(repo: Path) -> list[str]:
    errors: list[str] = []
    for relative in REQUIRED_FILES + LEGACY_FILES:
        if not (repo / relative).is_file():
            errors.append(f"missing required file: {relative}")
    for directory in REQUIRED_DIRS:
        if not (repo / directory / "README.md").is_file():
            errors.append(f"missing topic index: {directory}/README.md")

    current = (repo / "CURRENT.md")
    if current.is_file():
        text = current.read_text(encoding="utf-8")
        for marker in CURRENT_MARKERS:
            if marker not in text:
                errors.append(f"CURRENT.md missing marker: {marker}")

    environment = repo / "ENVIRONMENT.md"
    if environment.is_file():
        text = environment.read_text(encoding="utf-8")
        for field in ENVIRONMENT_FIELDS:
            if field not in text:
                errors.append(f"ENVIRONMENT.md missing field: {field}")

    for markdown in repo.rglob("*.md"):
        if ".git" in markdown.parts:
            continue
        text = FENCED_CODE.sub("", markdown.read_text(encoding="utf-8"))
        for target in LOCAL_LINK.findall(text):
            clean_target = target.split("#", 1)[0]
            if clean_target and not (markdown.parent / clean_target).resolve().exists():
                source = markdown.relative_to(repo).as_posix()
                errors.append(f"broken local link: {source} -> {target}")

    for path in tracked_files(repo):
        relative = path.relative_to(repo).as_posix()
        if path.suffix in {".o", ".d"}:
            errors.append(f"tracked build artifact: {relative}")
        elif path.is_file() and path.read_bytes()[:4] == b"\x7fELF":
            errors.append(f"tracked build artifact: {relative}")
    return sorted(set(errors))


def main() -> int:
    errors = validate_repository(Path(__file__).resolve().parents[1])
    if errors:
        print("Repository validation failed:")
        for error in errors:
            print(f"- {error}")
        return 1
    print("Repository validation passed.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
```

- [ ] **Step 4: Run the unit tests and confirm all five pass**

Run: `python3 -m unittest tests/test_validate_repository.py -v`

Expected: `Ran 5 tests` and `OK`.

- [ ] **Step 5: Run the validator against the current repository and record the expected contract failures**

Run: `python3 scripts/validate_repository.py`

Expected: exit 1 listing missing root documents/topic indexes and the currently tracked `.o`, `.d`, and ELF files. This proves the validator detects the work still required.

- [ ] **Step 6: Commit the green tests and validator together**

```bash
git add tests/test_validate_repository.py scripts/__init__.py scripts/validate_repository.py
git commit -m "build: add repository structure validator"
```

---

### Task 2: Add the learning control documents and topic indexes

**Files:**
- Create: `README.md`
- Create: `ROADMAP.md`
- Create: `CURRENT.md`
- Create: `MASTERY.md`
- Create: `ENVIRONMENT.md`
- Create: `TEACHING_PROTOCOL.md`
- Create: `daily/README.md`
- Create: `daily/TEMPLATE.md`
- Create: `linux/README.md`
- Create: `socket/README.md`
- Create: `cmake/README.md`
- Create: `ros2/README.md`
- Create: `slam/README.md`
- Create: `projects/README.md`
- Test: `tests/test_validate_repository.py`

**Interfaces:**
- Consumes: preserved `DAY1/`–`DAY6/` evidence and the approved design.
- Produces: the human-facing navigation, route, checkpoint, mastery ledger, environment record, teaching rules, and future content boundaries.

- [ ] **Step 1: Write the root README as the single entry point**

Include these exact sections and facts:

```markdown
# LINUX-STUDY

从 Linux 系统编程出发，逐步走向 ROS 2、SLAM 与移动机器人实践的长期学习仓库。

## 当前事实
- 仓库证据覆盖 DAY1–DAY6。
- 当前可确认主题：Linux 基础、Git/Makefile、文件 I/O、进程、IPC、semaphore、mutex。
- condition variable、rwlock 及后续阶段均须通过实际任务确认，不直接写成“已掌握”。

## 从这里开始
- [当前进度](CURRENT.md)
- [长期路线](ROADMAP.md)
- [掌握度标准](MASTERY.md)
- [实际环境](ENVIRONMENT.md)
- [教学协议](TEACHING_PROTOCOL.md)

## 历史学习记录
```

Under `历史学习记录`, link each DAY note and summarize only what its file actually contains. Under `后续主题`, link all seven topic indexes. Finish with the session workflow: read `CURRENT.md`, perform the next test without notes, learn/practice, update evidence, then commit.

- [ ] **Step 2: Write ROADMAP.md with prerequisite order and exit evidence**

Use ten phases matching the approved design. For every phase include:

```markdown
### 阶段 N：名称
**状态：** 当前 / 待验证 / 未开始
**学习范围：** concrete concepts
**完成证据：** one independent reproduction, one variation, one debugging task, and one practical output appropriate to the phase
**下一阶段门槛：** exact prerequisite evidence
```

Mark only Linux system programming as current. Mark later phases `未开始`; do not assign dates. Explicitly keep advanced server topics such as deep epoll/Reactor work outside the current robotics-critical path.

- [ ] **Step 3: Write CURRENT.md as a compact checkpoint**

Use these sections and claims:

```markdown
# CURRENT

最后更新：2026-09-06

## 当前阶段
Linux 系统编程收尾与并发基础验证。

## 仓库已验证
- DAY1–DAY6 contain notes/source exercises for the audited topics.

## 学习者自述
- condition variable 已学习。
- pthread read-write lock 正在学习。

## 待验证
- independently explain and reproduce condition-variable wait/signal flow;
- independently implement two readers and one writer with a read-write lock;
- diagnose the preserved mutex, mmap, FIFO, and shell issues listed in the design.

## 下一次测试
Without notes, explain the complete state transition of `pthread_cond_wait`, then write the synchronization skeleton.

## 下一步
Verify condition variables and read-write locks before starting the producer-consumer exercise.
```

Keep this file short; do not duplicate the full roadmap or daily notes.

- [ ] **Step 4: Write MASTERY.md, ENVIRONMENT.md, and TEACHING_PROTOCOL.md**

`MASTERY.md` must define L0–L5 exactly as the design, explain acceptable evidence for each promotion, and initialize the ledger conservatively: repository topics at L1–L2 unless independent reproduction is recorded, self-reported condition variable/rwlock as `待验证`, and all future stages L0.

`ENVIRONMENT.md` must contain fields for `操作系统`, `Linux 发行版/虚拟机`, `CPU 架构`, `编译器`, `CMake`, `Python`, `ROS 2`, `仿真器`, `开发板`, `底盘`, `电机驱动`, `编码器`, `IMU`, `LiDAR`, and `通信方式`. Use `待确认` for every value not evidenced in the repository. Record that checked-in binaries were ARM aarch64 artifacts only as repository evidence, not proof of the current machine.

`TEACHING_PROTOCOL.md` must turn all nine rules from the design into operational session rules, include the evidence hierarchy by domain, define the learning loop, define a chapter Definition of Done, and state that the learner retains final authority.

- [ ] **Step 5: Write focused topic indexes and the daily template**

Each index must state purpose, prerequisites, current evidence, completion evidence, and where new source goes. `linux/README.md` must link all preserved DAY notes. `daily/TEMPLATE.md` must contain only:

```markdown
# YYYY-MM-DD 学习记录

## 今日目标
## 闭卷回顾结果
## 今日真正掌握
## 做过的代码或实验
## 暴露的问题
## 下一次测试
## 证据与参考资料
```

- [ ] **Step 6: Run unit tests and the validator**

Run: `python3 -m unittest tests/test_validate_repository.py -v`

Expected: `Ran 5 tests` and `OK`.

Run: `python3 scripts/validate_repository.py`

Expected: exit 1 only because the audited build artifacts have not yet been removed.

- [ ] **Step 7: Commit the learning control layer**

```bash
git add README.md ROADMAP.md CURRENT.md MASTERY.md ENVIRONMENT.md TEACHING_PROTOCOL.md daily linux socket cmake ros2 slam projects
git commit -m "docs: add evidence-based robotics learning structure"
```

---

### Task 3: Remove audited build outputs and prevent recurrence

**Files:**
- Create: `.gitignore`
- Delete: `DAY2/Project_demo/bin/app`
- Delete: `DAY2/Project_demo/build/main.d`
- Delete: `DAY2/Project_demo/build/main.o`
- Delete: `DAY2/Project_demo/build/test1.d`
- Delete: `DAY2/Project_demo/build/test1.o`
- Delete: `DAY2/Project_demo/build/test2.d`
- Delete: `DAY2/Project_demo/build/test2.o`
- Delete: `DAY2/Project_demo/build/test3.d`
- Delete: `DAY2/Project_demo/build/test3.o`
- Delete: `DAY2/Project_demo/src/main.o`
- Delete: `DAY2/Project_demo/src/test1/test1.o`
- Delete: `DAY2/Project_demo/src/test2/test2.o`
- Delete: `DAY2/Project_demo/src/test3/test3.o`
- Delete: `DAY3/file_demo/main_file`
- Delete: `DAY3/file_demo/myls_main`
- Delete: `DAY3/file_demo/test2_main`
- Delete: `DAY4/myexec`
- Delete: `DAY4/myshell`
- Delete: `DAY4/myshell_main`
- Delete: `DAY5/mmap_main`
- Delete: `DAY5/mmap_test/read`
- Delete: `DAY5/mmap_test/read_main`
- Delete: `DAY5/mmap_test/write_main`
- Delete: `DAY6/mutex`
- Delete: `DAY6/sem`
- Test: `tests/test_validate_repository.py`

**Interfaces:**
- Consumes: the audited artifact list and validator.
- Produces: a source-only tracked tree protected by ignore rules.

- [ ] **Step 1: Add exact ignore rules**

```gitignore
# C/C++ build products
*.o
*.d
*.a
*.so
*.out
build/
bin/

# CMake
CMakeFiles/
CMakeCache.txt
cmake_install.cmake
compile_commands.json

# ROS 2 / colcon
install/
log/

# Editors and operating systems
.vscode/
.idea/
.DS_Store

# Generated robot data
*.bag
*.db3
*.pgm
*.yaml.tmp
```

- [ ] **Step 2: Remove only the 25 explicit files listed above**

Use `git rm` with the exact paths from this task. Do not use a recursive wildcard. Preserve `DAY5/mmap_test/shared.dat`, all `.txt` fixtures, every source/header/Makefile, and every Markdown note.

- [ ] **Step 3: Verify the cleanup contract**

Run: `python3 scripts/validate_repository.py`

Expected: `Repository validation passed.`

Run: `git ls-files -z | xargs -0 file | grep -E 'ELF|Mach-O'`

Expected: no output and grep exit code 1.

- [ ] **Step 4: Commit the cleanup**

```bash
git add .gitignore
git commit -m "chore: stop tracking generated build artifacts"
```

---

### Task 4: Final repository verification and handoff

**Files:**
- Modify only if verification exposes a concrete defect in the files from Tasks 1–4.
- Test: `tests/test_validate_repository.py`

**Interfaces:**
- Consumes: all prior task outputs.
- Produces: verification evidence and a reviewable branch ready for user-authorized push or merge.

- [ ] **Step 1: Run the full automated verification**

Run: `python3 -m unittest discover -s tests -v`

Expected: all tests pass with `OK`.

Run: `python3 scripts/validate_repository.py`

Expected: `Repository validation passed.`

- [ ] **Step 2: Run Git integrity checks**

Run: `git diff --check origin/main...HEAD`

Expected: no output and exit code 0.

Run: `git status --short --branch`

Expected: branch `codex/reorganize-learning-repo` with a clean worktree.

Run: `git merge-base --is-ancestor origin/main HEAD`

Expected: exit code 0, proving the work is based on the audited remote head.

- [ ] **Step 3: Review preservation and scope**

Run: `git diff --name-status origin/main...HEAD`

Confirm all `DAY1/`–`DAY6/` Markdown and source files remain, only the 25 audited build outputs are deleted, and all new files match the approved architecture.

- [ ] **Step 4: Report the branch without pushing or merging**

Summarize verified evidence, conservative progress labels, route correction, removed generated files, test results, branch name, and commits. Explicitly request separate authorization before pushing to GitHub or merging into `main`.
