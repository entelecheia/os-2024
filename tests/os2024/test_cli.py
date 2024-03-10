"""
test cli module
"""

import subprocess
from typing import List, Tuple


def capture(command: List[str]) -> Tuple[bytes, bytes, int]:
    proc = subprocess.Popen(
        command,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    out, err = proc.communicate()
    return out, err, proc.returncode


def test_cli() -> None:
    """Test cli module"""
    command = ["os2024"]
    out, err, exitcode = capture(command)
    assert exitcode == 0


def test_cli_local() -> None:
    """Test cli module"""
    command = ["os2024", "--local"]
    out, err, exitcode = capture(command)
    assert exitcode == 0


def test_cli_build() -> None:
    """Test cli module"""
    command = ["os2024", "--build"]
    out, err, exitcode = capture(command)
    assert exitcode == 0
