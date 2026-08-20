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
PER_FUNCTION_CAP = 8000
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




def probe_recomp_bytes():
    import struct
    exe = os.path.join(os.path.dirname(__file__), "..", "recompiled", "th08.exe")
    probes = [
        ("DrawCompletionStatusText recomp VA 0x4519fe", 0x4519FE, 0x360),
    ]
    if not os.path.exists(exe):
        print("probe: exe not found"); return
    with open(exe, "rb") as f:
        data = f.read()
    pe = struct.unpack_from("<I", data, 0x3C)[0]
    image_base = struct.unpack_from("<I", data, pe + 24 + 28)[0]
    nsec = struct.unpack_from("<H", data, pe + 6)[0]
    opt = struct.unpack_from("<H", data, pe + 20)[0]
    sec0 = pe + 24 + opt
    for label, va, nbytes in probes:
        rva = va - image_base
        for i in range(nsec):
            off = sec0 + i * 40
            vsize, vaddr, rawsize, rawptr = struct.unpack_from("<IIII", data, off + 8)
            if vaddr <= rva < vaddr + max(vsize, rawsize):
                fo = rawptr + (rva - vaddr)
                print("probe %s bytes=%s" % (label, data[fo:fo + nbytes].hex()))
                break
        else:
            print("probe %s NOT MAPPED" % label)



def probe_recomp_symbols():
    import csv as csvmod
    import subprocess as spmod
    out = spmod.run(["reccmp-roadmap", "--target", "th08", "--csv", "roadmap_probe.csv"],
                    capture_output=True, text=True, timeout=600)
    if out.returncode != 0:
        print("roadmap probe failed:", out.stderr[-1500:]); return
    rows = []
    with open("roadmap_probe.csv") as f:
        for row in csvmod.DictReader(f):
            if row.get("row_type") not in ("dat", "poi", "vta", "str", "flo", "off", "lab", "imp"):
                continue
            name = row.get("name") or ""
            orig = row.get("orig_addr") or ""
            rec = row.get("recomp_addr") or ""
            if not orig or not rec or not name:
                continue
            try:
                o = int(orig, 16) if orig.startswith("0x") else int(orig)
                r = int(rec, 16) if rec.startswith("0x") else int(rec)
            except ValueError:
                continue
            rows.append((o, r, int(row.get("size") or 0), name))
    rows.sort()
    prev_o = prev_r = None
    for o, r, size, name in rows:
        gap_note = ""
        if prev_o is not None:
            go, gr = o - prev_o, r - prev_r
            if go != gr:
                gap_note = "   <== GAPDIFF orig+%d recomp+%d (missing %d bytes)" % (go, gr, go - gr)
        print("sym %-46s orig=0x%08x rec=0x%08x sz=%#x%s" % (name, o, r, size, gap_note))
        prev_o, prev_r = o, r

def main():
    try:
        probe_recomp_symbols()
    except Exception as e:
        print("symbol probe error:", e)

    try:
        probe_recomp_bytes()
    except Exception as e:
        print("probe error:", e)

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
