#!/usr/bin/env python3
"""Disassemble a function from th08.exe by virtual address (objdump-based).

Usage:
    python3 scripts/disasm.py 0x446232                 # auto length
    python3 scripts/disasm.py 0x446232 0x296           # explicit length in bytes
    python3 scripts/disasm.py 0x446232 --end 0x4464c8  # explicit end address

Requires: binutils objdump with pei-i386 support, and the original
resources/th08.exe (not committed to the repository).
"""
import struct
import subprocess
import sys
import re

EXE = "resources/th08.exe"


def pe_info(path):
    with open(path, "rb") as f:
        data = f.read(0x2000)
    pe_off = struct.unpack_from("<I", data, 0x3C)[0]
    assert data[pe_off:pe_off + 4] == b"PE\0\0", "not a PE file"
    image_base = struct.unpack_from("<I", data, pe_off + 24 + 28)[0]
    return image_base


def main():
    exe = EXE
    args = [a for a in sys.argv[1:] if not a.startswith("--exe=")]
    for a in sys.argv[1:]:
        if a.startswith("--exe="):
            exe = a.split("=", 1)[1]

    start = int(args[0], 16)
    if len(args) >= 2 and args[1] != "--end":
        end = start + int(args[1], 16)
    elif "--end" in args:
        end = int(args[args.index("--end") + 1], 16)
    else:
        end = start + 0x400

    # objdump VMA == PE image base + RVA already; dump only the code section.
    out = subprocess.run(
        [
            "objdump", "-d", "-Mintel",
            "--start-address=0x%x" % start,
            "--stop-address=0x%x" % end,
            exe,
        ],
        capture_output=True,
        text=True,
    ).stdout

    insn_re = re.compile(r"^\s*([0-9a-f]+):\s+((?:[0-9a-f]{2} )+)\s+(\S+)(?:\s+(.*))?$")
    count = 0
    for line in out.splitlines():
        m = insn_re.match(line)
        if not m:
            continue
        addr, bytes_, mnemonic, ops = m.group(1), m.group(2).strip(), m.group(3), m.group(4) or ""
        print("0x%08x  %-32s %-10s %s" % (int(addr, 16), bytes_, mnemonic, ops))
        count += 1
    if count == 0:
        sys.exit("no instructions decoded; check address range and exe path (%s)" % exe)


if __name__ == "__main__":
    main()
