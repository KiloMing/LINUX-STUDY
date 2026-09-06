# Linux-to-Robotics Learning Repository Reorganization Design

## Purpose

Turn the existing `LINUX-STUDY` repository into a durable learning record for the path from Linux system programming to mobile robotics, without erasing the learner's original notes or overstating progress.

The repository must serve four roles:

1. preserve original study notes and source exercises;
2. show the current learning checkpoint without depending on chat history;
3. define a technically ordered long-term route;
4. store future exercises and projects in predictable topic directories.

## Evidence Used

This design is based on the repository at commit `1dc2bde` on 2026-09-06 and the recorded learning-route discussion supplied by the user.

Repository evidence currently covers:

- `DAY1/`: basic Linux commands, Vim, SSH, and Git notes;
- `DAY2/`: Makefile notes and a multi-file C++ Makefile exercise;
- `DAY3/`: file I/O, directory traversal, and pipe exercises;
- `DAY4/`: process, `fork`, `exec`, `wait`, and a minimal shell exercise;
- `DAY5/`: FIFO, `mmap`, and IPC exercises plus a dated note;
- `DAY6/`: semaphore and mutex exercises.

The repository does not currently contain sufficient evidence to mark condition variables, read-write locks, Socket, CMake, ROS 2, SLAM, or Nav2 as mastered. Conversation-only progress may be recorded as self-reported and awaiting verification, never as repository-verified mastery.

## Constraints

- Do not rewrite Git history or force-push.
- Preserve `DAY1/` through `DAY6/` and the learner's original writing style.
- Do not perform a mass rename or migration of old paths.
- Do not delete source code, notes, or teaching inputs.
- Generated binaries and object/dependency files may be removed from the current tree because they are reproducible and remain available in Git history.
- Unknown operating-system, toolchain, ROS, and hardware values must be written as `待确认`, not guessed.
- Existing questionable code is teaching evidence. Do not silently repair it during repository reorganization; surface it as future debugging work.
- Future technical claims that may change with versions must be checked against primary documentation before use.

## Information Architecture

### Root control documents

- `README.md`: repository purpose, present evidence, navigation, daily workflow, and contribution rules.
- `ROADMAP.md`: stable, prerequisite-aware route and phase completion criteria.
- `CURRENT.md`: small, frequently updated checkpoint containing verified state, self-reported state, next recall test, blockers, and next action.
- `MASTERY.md`: L0-L5 rubric plus evidence rules and a topic ledger.
- `ENVIRONMENT.md`: confirmed host, compiler, build, ROS, and hardware versions; all unknown fields remain `待确认`.
- `TEACHING_PROTOCOL.md`: evidence hierarchy, anti-sycophancy rule, review-first workflow, worked-example fading, spaced retrieval, Definition of Done, anti-black-box expectations, and route-change rules.
- `.gitignore`: prevent new build outputs, editor files, ROS build trees, and generated maps/logs from being committed.

### Content directories

- `daily/`: new date-based learning logs and a compact template; old DAY notes stay in place and are indexed.
- `linux/`: index to existing DAY material and home for future Linux exercises.
- `socket/`: TCP/UDP learning scope and future exercises.
- `cmake/`: modern target-based CMake learning scope and future exercises.
- `ros2/`: ROS 2 fundamentals, debugging, TF2, URDF, RViz, ros2_control, and sensor integration indexes.
- `slam/`: odometry, state estimation, mapping, localization, SLAM, and Nav2 indexes.
- `projects/`: milestone projects that combine prior topics.

Empty directories will not be represented by placeholder files alone. Each directory receives a focused `README.md` explaining its boundary, prerequisites, intended evidence, and current status.

## Roadmap Design

The route is ordered by prerequisites rather than by technology popularity:

1. finish Linux concurrency fundamentals and debug existing exercises;
2. learn the Socket subset needed for robot-to-host and sensor communication;
3. learn modern CMake for maintainable multi-target C++ projects;
4. learn ROS 2 workspace, package, node, topic, service, action, parameter, launch, and command-line debugging;
5. learn TF2, URDF, RViz, and simulation;
6. learn differential-drive kinematics and odometry before depending on a finished physical base;
7. integrate STM32 communication, encoders, motors, feedback control, and ros2_control;
8. integrate IMU and LiDAR, timestamps, frames, and state estimation;
9. perform mapping/localization with SLAM, then navigation with Nav2;
10. add higher-level perception or application features only when a project requires them.

This corrects the earlier coarse order that placed kinematics and odometry only after the physical chassis. Simulation and mathematical models should validate the motion model before hardware integration; real hardware must still follow so simulation does not become the final result.

Each phase in `ROADMAP.md` will define an observable exit condition. Merely reading a tutorial or launching an existing package is not completion.

## State and Mastery Model

`CURRENT.md` and `MASTERY.md` use separate evidence labels:

- **仓库已验证**: supported by committed notes/code and, when appropriate, a reproducible check;
- **学习者自述**: reported in conversation but not yet demonstrated in this repository;
- **待验证**: requires a recall, modification, or debugging task;
- **未开始**: no evidence found.

Mastery levels are:

- L0: not studied;
- L1: can recognize terminology with help;
- L2: can explain the model and follow a complete example;
- L3: can reproduce a minimal solution without copying;
- L4: can adapt the solution and diagnose common failures;
- L5: can design, justify, test, and review an integrated solution independently.

No topic is promoted from a single correct answer. Promotion requires evidence appropriate to the level, and L3 or above requires independent work. A failed first attempt becomes a test candidate, not automatically a permanent knowledge gap.

## Teaching Protocol

The teaching protocol will enforce these rules:

1. Truth and reproducible evidence outrank agreement or confidence.
2. Primary sources are preferred: standards and manual pages for Linux/POSIX, official documentation for CMake/ROS/Nav2, data sheets and vendor references for hardware, and papers or authoritative texts for SLAM algorithms.
3. Version-sensitive advice must be checked against `ENVIRONMENT.md`; if the environment is unknown, stop at a version-neutral explanation or request confirmation.
4. Each session starts with `CURRENT.md` and a short unassisted retrieval task from the last session.
5. New material follows: model → complete worked example → fading hints → independent reproduction → variation → deliberate fault/debugging → practical use → delayed retrieval.
6. Running an example is not mastery. The learner must identify inputs, outputs, dependencies, configuration, and failure modes.
7. Necessary prerequisites cannot be skipped merely by preference, and low-value depth can be stopped when it does not serve the robotics goal.
8. The route may change only because evidence shows a prerequisite is already mastered, a real project need appears, the route is technically wrong, the ecosystem changes, the cost-benefit is poor, or assessment exposes a missing prerequisite.
9. The learner retains final authority over their repository and goals; the protocol grants no authority beyond the learning workflow.

## Existing Code Findings to Preserve as Debug Work

The reorganization will not claim these issues are fixed:

- the process-shared mutex exercise does not visibly initialize a process-shared mutex attribute;
- the `mmap` exercise maps 256 bytes but uses the original file size for `munmap`;
- the FIFO reader can continue looping after EOF and may print stale buffer contents;
- the minimal shell assumes non-empty input before accessing the first argument;
- the Makefile exercise expects generated output directories to exist and has tracked build products.

These become concrete candidates for later reproduce-explain-fix tests. The list is not a complete code review.

## Generated Artifact Cleanup

The implementation may remove tracked files identified as compiled ELF executables, `.o` objects, and `.d` dependency files. It must first identify them by file type or extension and review the exact list. It must retain:

- all source and Markdown files;
- small text fixtures such as `test.txt` and `dir.txt`;
- IPC fixture files unless they are demonstrably generated and unnecessary;
- all historical availability through existing Git commits.

No broad wildcard deletion is allowed. Removal targets must be explicit.

## Verification

The reorganized repository will include a lightweight, dependency-free validation script and tests written before the script. Validation will check:

- required root documents and topic directories exist;
- Markdown links that point to local files resolve;
- `CURRENT.md` uses evidence labels and contains the next test/action;
- `ENVIRONMENT.md` does not silently omit unknown required fields;
- tracked compiled artifacts are absent;
- legacy DAY directories and their source/notes remain present.

Final verification also includes:

- running the repository validation test suite;
- running the validation script against the repository;
- reviewing `git diff --check`;
- reviewing the final tracked-file list and Git status;
- confirming the branch is based on the current `origin/main` commit.

## Delivery

Changes remain on `codex/reorganize-learning-repo` until explicitly pushed or merged. The implementation will use small, reviewable commits and will not alter `main` directly.
