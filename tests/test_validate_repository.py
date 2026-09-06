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
                "# Document\n```markdown\n[example](not-a-real-file.md)\n```\n",
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
