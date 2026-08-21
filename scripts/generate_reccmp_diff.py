import json
import os
import re
import subprocess
import sys
import tempfile

# Addresses/names listed in config/verbose_watch.txt get a full per-instruction
# reccmp diff dump written to verbose_diff.txt (picked up by compare.yml and
# posted as part of the commit comment). One entry per line; use reccmp names
# or original addresses (0x...). Lines starting with # are ignored.

WATCHLIST = os.path.join(os.path.dirname(__file__), "..", "config", "verbose_watch.txt")
VERBOSE_OUT = "verbose_diff.txt"
ANSI_RE = re.compile(r"\x1b\[[0-9;]*m")
PER_FUNCTION_CAP = 24000
TOTAL_CAP = 46000


def list_watch():
    if not os.path.exists(WATCHLIST):
        return []
    out = []
    with open(WATCHLIST) as f:
        for line in f:
            line = line.strip()
            if line and not line.startswith("#"):
                out.append(line)
    return out


def dump_verbose(entries):
    written = 0
    with open(VERBOSE_OUT, "w", errors="replace") as out:
        for entry in entries:
            if written >= TOTAL_CAP:
                break
            try:
                # reccmp's 100%-match banner contains a ✨ which crashes on
                # the Windows runner's cp1252 console - force UTF-8 I/O.
                env = dict(os.environ, PYTHONIOENCODING="utf-8", PYTHONUTF8="1")
                result = subprocess.run(
                    ["reccmp-reccmp", "--target", "th08", "--verbose", entry],
                    capture_output=True,
                    text=True,
                    errors="replace",
                    timeout=120,
                    env=env,
                )
                text = ANSI_RE.sub("", result.stdout + result.stderr)
            except Exception as e:
                text = "verbose dump failed: %s" % e
            text = text[:PER_FUNCTION_CAP]
            out.write("## verbose: %s\n```\n%s\n```\n" % (entry, text))
            written += len(text)


def main():
    report_filename = tempfile.mktemp()

    subprocess.run(
        ["reccmp-reccmp", "--target", "th08", "--json", report_filename],
        check=True,
        stdout=subprocess.DEVNULL,
    )

    reccmp_data = ""

    with open(report_filename, "r") as f:
        reccmp_data = json.load(f)

    print("# Report")
    print()

    rows = reccmp_data["data"]
    perfect = [f for f in rows if f["matching"] >= 0.999999]
    imperfect = [f for f in rows if f["matching"] < 0.999999]

    print("functions compared: %d | 100%% matches: %d | remaining: %d" % (len(rows), len(perfect), len(imperfect)))
    print()
    # Only list imperfect rows: the compare.yml commit-comment budget (55000
    # chars) is shared with the verbose dumps below, and the full table
    # crowded them out when nearly everything matched.
    print("name | result")
    print("-----|-------")

    for function in imperfect:
        print(function["name"] + " | " + format(function["matching"], ".2%"))

    dump_verbose(list_watch())


if __name__ == "__main__":
    main()
