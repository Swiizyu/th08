#!/usr/bin/env python3
"""Regenerate PROGRESS.md from the latest 'Compare reimplementation' commit comment.

Usage: python3 scripts/update_progress.py [commit-sha]
Without an argument, uses the current HEAD.
"""
import json
import re
import subprocess
import sys
import datetime

REPO = "Swiizyu/th08"
BRANCH = "arena/01a01c02-th08"


def sh(args):
    return subprocess.run(args, capture_output=True, text=True, check=True).stdout


def get_sha():
    if len(sys.argv) > 1:
        return sys.argv[1]
    return sh(["git", "rev-parse", "HEAD"]).strip()


def bar(frac, width=34):
    filled = int(round(frac * width))
    return "█" * filled + "░" * (width - filled)


def main():
    sha = get_sha()
    raw = sh(["gh", "api", "repos/%s/commits/%s/comments" % (REPO, sha), "--jq", "last(.[].body)"])
    m = re.search(r"functions compared: (\d+) \| 100% matches: (\d+) \| remaining: (\d+)", raw)
    if not m:
        print("no compare comment found on", sha)
        return 1
    total, perfect, remaining = int(m.group(1)), int(m.group(2)), int(m.group(3))

    pcts = [float(x) for x in re.findall(r"\| (\d+\.\d+)%\s*$", raw, re.M)]
    weighted = (perfect * 100.0 + sum(pcts)) / total
    exact = 100.0 * perfect / total

    # history: keep previous snapshots from existing file
    try:
        old = open("PROGRESS.md").read()
        hist = re.search(r"<!-- HISTORY -->(.*?)<!-- /HISTORY -->", old, re.S)
        rows = [ln for ln in (hist.group(1).strip().splitlines() if hist else []) if ln.strip()]
    except FileNotFoundError:
        rows = []

    today = datetime.datetime.now().strftime("%Y-%m-%d %H:%M")
    row = "| %s | `%s` | %d | %d | %.2f%% | %.2f%% |" % (today, sha[:7], total, perfect, weighted, exact)
    if not any(("`%s`" % sha[:7]) in r for r in rows):
        rows.append(row)
        rows = rows[-12:]

    doc = """# ⛩️ th08.exe — прогресс декомпиляции (живой)

Автообновление: `python3 scripts/update_progress.py` (данные — из CI-комментариев reccmp).
Ветка: [`%s`](https://github.com/%s/tree/%s) · CI-отчёты: [PR #2](https://github.com/%s/pull/2)

## Точное совпадение функций (100%% байт-в-байт)

```
%s  %.2f%%
```

**%d / %d** функций полностью идентичны оригинальному `th08.exe` v1.00d (SHA256 `330fbdbf…`). Снимок: `%s`, %s (UTC).

## Общий прогресс (с учётом частичных совпадений)

```
%s  %.2f%%
```

Каждая функция вносит свой процент: почти готовые (95-99%%) уже полностью разобраны и ждут последнего штриха.

## История снимков

| дата (UTC) | коммит | сравнивается | 100%% | общий | точных |
|---|---|---|---|---|---|
<!-- HISTORY -->
%s
<!-- /HISTORY -->

## Кто что делает

| агент | зона | ветка |
|---|---|---|
| ⛩️ Канако | Player (бомбы), Supervisor, Background, AnmManager | `arena/01a01c02-th08` |
| 🐸 Санаэ | Gui / TitleScreen / ResultScreen / GameManager | `arena/01a01ccf-th08` |
| 🌀 Сувако | Ending / AsciiManager / MusicRoom / RetryMenu | `arena/01a01cd1-th08` |

*Процент считается инструментом reccmp по 1365 сравниваемым единицам (функции + данные). Несоизмеримые единицы (линкерные свёртки CRT) в статистику не входят.*
""" % (BRANCH, REPO, BRANCH, REPO, bar(exact / 100.0), exact, perfect, total, sha[:7], today, bar(weighted / 100.0), weighted, "\n".join(rows))

    with open("PROGRESS.md", "w") as f:
        f.write(doc)
    print("PROGRESS.md updated: %d/%d (%.2f%% exact, %.2f%% weighted) @ %s" % (perfect, total, exact, weighted, sha[:7]))
    return 0


if __name__ == "__main__":
    sys.exit(main())
