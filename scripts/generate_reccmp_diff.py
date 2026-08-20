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
PER_FUNCTION_CAP = 30000
TOTAL_CAP = 32000


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
                result = subprocess.run(
                    ["reccmp-reccmp", "--target", "th08", "--verbose", entry],
                    capture_output=True,
                    text=True,
                    errors="replace",
                    timeout=120,
                )
                text = ANSI_RE.sub("", result.stdout + result.stderr)
            except Exception as e:
                text = "verbose dump failed: %s" % e
            text = text[:PER_FUNCTION_CAP]
            out.write("## verbose: %s\n```\n%s\n```\n" % (entry, text))
            written += len(text)


def probe_recomp_bytes():
    """Debug probe: dump raw bytes from the recompiled exe at fixed RVAs.

    Used to settle whether a mismatching immediate is a real constant
    difference or a BSS symbol-address shift. Entries: (label, rva, nbytes).
    """
    import struct

    exe = os.path.join(os.path.dirname(__file__), "..", "recompiled", "th08.exe")
    # (label, rva, nbytes) — set as needed for the current investigation.
    probes = [
        ("OnUpdateOptions color push @recomp 0x44a041", 0x44A041, 8),
    ]
    if not os.path.exists(exe):
        print("probe: %s not found" % exe)
        return
    with open(exe, "rb") as f:
        data = f.read()
    pe = struct.unpack_from("<I", data, 0x3C)[0]
    nsec = struct.unpack_from("<H", data, pe + 6)[0]
    opt_size = struct.unpack_from("<H", data, pe + 20)[0]
    sec0 = pe + 24 + opt_size
    for i in range(nsec):
        off = sec0 + i * 40
        vsize, vaddr, rawsize, rawptr = struct.unpack_from("<IIII", data, off + 8)
        print("section %-8s va=0x%08x vsize=0x%x rawsize=0x%x" %
              (data[off:off + 8].rstrip(b'\0').decode(), vaddr, vsize, rawsize))
    for label, rva, nbytes in probes:
        for i in range(nsec):
            off = sec0 + i * 40
            vsize, vaddr, rawsize, rawptr = struct.unpack_from("<IIII", data, off + 8)
            if vaddr <= rva < vaddr + max(vsize, rawsize):
                fo = rawptr + (rva - vaddr)
                print("probe %-44s rva=0x%08x bytes=%s" % (label, rva, data[fo:fo + nbytes].hex()))
                break
        else:
            print("probe %-44s rva=0x%08x NOT MAPPED" % (label, rva))


def probe_recomp_symbols():
    """Debug probe: dump orig-vs-recomp addresses of data (BSS) symbols via
    reccmp-roadmap, to find BSS layout shifts between the two binaries."""
    import csv as csvmod
    import subprocess as spmod

    out = spmod.run(
        ["reccmp-roadmap", "--target", "th08", "--csv", "roadmap_probe.csv"],
        capture_output=True, text=True, timeout=600,
    )
    if out.returncode != 0:
        print("roadmap probe failed:", out.stderr[-2000:])
        return
    rows = []
    with open("roadmap_probe.csv") as f:
        for row in csvmod.DictReader(f):
            if row.get("row_type") not in ("dat", "poi", "vta", "str", "flo", "off", "lab", "imp"):
                continue
            name = row.get("name") or ""
            orig = row.get("orig_addr") or ""
            rec = row.get("recomp_addr") or ""
            if not orig or not rec:
                continue
            try:
                o, r = int(orig, 16 if orig.startswith("0x") else 10), int(rec, 16 if rec.startswith("0x") else 10)
            except ValueError:
                continue
            rows.append((o, r, row.get("size") or "", name))
    rows.sort()
    for o, r, size, name in rows:
        delta = r - o
        flag = "  <== SHIFT %d (0x%x)" % (delta, delta & 0xFFFFFFFF) if delta else ""
        print("sym %-40s orig=0x%08x recomp=0x%08x size=%s%s" % (name, o, r, size, flag))


def main():
    try:
        probe_recomp_bytes()
    except Exception as e:
        print("byte probe error:", e)
    try:
        probe_recomp_symbols()
    except Exception as e:
        print("symbol probe error:", e)
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
    total = 0
    perfect = 0
    imperfect = []
    for function in reccmp_data["data"]:
        total += 1
        if function["matching"] >= 1.0:
            perfect += 1
        else:
            imperfect.append(function)

    print("%d/%d functions at 100.00%%; %d below:" % (perfect, total, len(imperfect)))
    print()
    print("name | result")
    print("-----|-------")

    for function in imperfect:
        print(function["name"] + " | " + format(function["matching"], ".2%"))

    dump_verbose(list_watch())


if __name__ == "__main__":
    main()
