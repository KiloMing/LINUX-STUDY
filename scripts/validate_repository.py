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

    current = repo / "CURRENT.md"
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
