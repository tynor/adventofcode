import shlex
import subprocess
from typing import Any


def Settings(language: str, **kwargs: Any) -> dict[str, Any]:
    if language == "cfamily":
        flags_line = subprocess.check_output(["./flags.sh"], encoding="ascii")
        flags = [
            *shlex.split(flags_line),
            "-x",
            "c",
            "-Wno-empty-translation-unit",
            "-Wno-undefined-internal",
        ]
        return {
            "flags": flags,
        }

    return {}
