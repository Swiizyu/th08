#!/usr/bin/env python3
"""Find orig-side absolute refs (mov [addr], call addr) in a function that are
NOT covered by any reccmp CSV row yet. Candidates for name fishing.

Usage:
    python3 scripts/find_unnamed_refs.py 0x4664a0 [--end 0x466650]
"""
import csv
import re
import subprocess
import sys

EXE = "resources/th08.exe"
CSV_FILES = [
    "config/reccmp-functions.csv",
    "config/reccmp-globals.csv",
    "config/reccmp-strings.csv",
    "config/reccmp-floats.csv",
]


def load_known():
    known = {}
    for path in CSV_FILES:
        try:
            with open(path, newline="") as f:
                for line in f:
                    line = line.strip()
                    if not line or line.startswith("#") or line.startswith("//"):
                        continue
                    parts = next(csv.reader([line]))
                    if len(parts) < 2:
                        continue
                    addr_field = parts[1] if parts[1].startswith("0x") else parts[0]
                    if not addr_field.lower().startswith("0x"):
                        continue
                    try:
                        addr = int(addr_field, 16)
                    except ValueError:
                        continue
                    name = parts[2] if (parts[1].startswith("0x")) else parts[1]
                    name = parts[0] if addr_field == parts[0] else name
                    known.setdefault(addr, name)
        except FileNotFoundError:
            pass
    return known


def covered(known, addr):
    # exact or within any known global's plausible extent
    if addr in known:
        return known[addr]
    for base, name in known.items():
        if addr >= base and addr - base < 0x1000 and not name.startswith("0x"):
            return f"{name}+0x{addr-base:x}"
    return None


def main():
    start = int(sys.argv[1], 16)
    if "--end" in sys.argv:
        end = int(sys.argv[sys.argv.index("--end") + 1], 16)
    else:
        end = start + 0x400
    known = load_known()
    out = subprocess.run(
        ["objdump", "-d", "-Mintel",
         "--start-address=0x%x" % start, "--stop-address=0x%x" % end,
         EXE, "-w"],
        capture_output=True, text=True, check=True).stdout
    refs = {}
    for line in out.splitlines():
        m = re.match(r"\s*([0-9a-f]+):\s+((?:[0-9a-f]{2} )+)\s*(.*)", line)
        if not m:
            continue
        addr, asm = int(m.group(1), 16), m.group(3)
        for target in re.findall(r"(?:call|jmp)\s+(?:DWORD PTR )?(ds:)?0x([0-9a-f]{5,8})\b", asm):
            t = int(target[1], 16)
            if 0x400000 <= t < 0x600000:
                refs.setdefault(t, []).append((addr, asm))
        for t_s in re.findall(r"\[0x([0-9a-f]{5,8})\]", asm):
            t = int(t_s, 16)
            if 0x400000 <= t < 0x600000:
                refs.setdefault(t, []).append((addr, asm))
    for t in sorted(refs):
        cov = covered(known, t)
        tag = "KNOWN " + cov if cov else "=== UNNAMED"
        print(f"{tag} 0x{t:x}  (refs: {len(refs[t])})")
        if not cov:
            for a, asm in refs[t][:3]:
                print(f"      {a:x}: {asm}")


if __name__ == "__main__":
    main()
