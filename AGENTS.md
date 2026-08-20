# AGENTS.md — multi-agent coordination for th08 decompilation

Goal: byte-exact decompilation of `th08.exe` v1.00d (Touhou 8, Imperishable Night).
Metric: number of functions at **100.00%** in the reccmp compare table (see §4).
Never claim a function is done until the compare comment says `100.00%`.

## 1. Agents and branches

| Agent | Arena chat | Branch (auto) | Owns |
|-------|-----------|---------------|------|
| Agent 1 (integration owner) | chat 1 | `arena/01a01c02-th08` | `src/AnmManager.*`, `src/Supervisor.*`, `src/Player.*`, `src/Background.*`, math in shared headers, integration of PRs |
| Agent 2 | chat 2 | `arena/XXXXXXXX-th08` (auto-assigned) | `src/Gui.*`, `src/MusicRoom.*`, `src/ResultScreen.*`, `src/TitleScreen.*`, `src/Ending.*`, `src/GameManager.*`, `src/AsciiManager.*` |

Shared files → **append-only, keep rows sorted by address** to avoid merge conflicts:
`config/reccmp-functions.csv`, `config/reccmp-globals.csv`, `config/globals.csv`,
`config/reccmp-floats.csv`, `config/verbose_watch.txt`.
Shared base headers (`src/*.hpp` types used by both slices): ask in the sync
channel before editing (§3).

## 2. Hard rules (both agents)

1. Work ONLY on your own auto-branch. Never merge to `main`, never force-push.
2. Never push changes under `.github/workflows/` (bot token is denied there anyway).
3. Never commit `*.exe` (gitignored). The original binary must stay out of git.
4. Commits must be clean: run `git diff --check`. No `MATCHED`/100% claims without
   a compare comment proving `100.00%`.
5. Agent 2 integrates via PRs **into `arena/01a01c02-th08`** (base = that branch,
   NOT main). Agent 1 reviews and merges.
6. Keep `// FUNCTION: th08 0x<addr>` marker comments above every reconstructed
   function; they are the address source for reccmp pairing.

## 3. Sync channel (two-way)

Issues are disabled on the repo. Use **comments on PR #2**:

- Read at the START of every turn:
  `gh api repos/Swiizyu/th08/issues/2/comments --jq '.[-6:][] | .user.login + ": " + .body'`
- Post: `gh pr comment 2 -R Swiizyu/th08 --body "CLAIM: src/Gui.cpp OnUpdateOptions"`
- Message types: `CLAIM: <scope>` (before starting), `DONE: <scope> (commit sha, score)`,
  `BLOCKED: <what/why>`, `NOTE: <discovery worth sharing>`.
- If `gh pr comment` ever fails for an agent, fall back to committing `SYNC-<branch>.md`
  notes on your own branch and say so in your final user message.

## 4. Verify loop (the only source of truth)

1. `git commit` → `git push origin <your-branch>`.
2. Push triggers **Build binary** (~1.5 min), then **Compare reimplementation** (~1 min).
3. The compare posts a table as a **commit comment on your head sha**:
   `gh api repos/Swiizyu/th08/commits/$(git rev-parse HEAD)/comments --jq 'last(.[]).body'`
4. Rows missing from the table = recomp function name ≠ CSV name for that address
   (reccmp pairs by demangled base name). Add/adjust a row in
   `config/reccmp-functions.csv` (append-only) OR rename your function to the CSV name.
5. Functions whose original references raw addresses of globals need rows in
   `config/reccmp-globals.csv` + `config/globals.csv`, otherwise the two sides print
   `<OFFSET_N>` vs a name and never match.

## 5. Sandbox/bootstrap notes (Agent 2)

- The sandbox wipes everything not committed between turns. Commit often.
- `resources/th08.exe` is gitignored and may vanish; re-fetch:
  ```
  SHA=$(curl -s "https://api.github.com/repos/1warriorscats1-sys/-/git/trees/HEAD" | \
        python3 -c "import json,sys; print([t['sha'] for t in json.load(sys.stdin)['tree'] if t['path']=='th08.exe'][0])")
  curl -s "https://api.github.com/repos/1warriorscats1-sys/-/git/blobs/$SHA" | \
        python3 -c "import json,sys,base64; open('resources/th08.exe','wb').write(base64.b64decode(json.load(sys.stdin)['content']))"
  ```
  Verify: sha256 must be `330fbdbf58a710829d65277b4f312cfbb38d5448b3df523e79350b879213d924`.
  Do NOT delete the burner repo `1warriorscats1-sys/-` (CI downloads the exe from it).
- Only github.com/api.github.com/PyPI are reachable from the sandbox.
- No MSVC locally: the only compiler is CI (MSVC 2002). Disassemble locally, compile in CI.
- Disasm: `python3 scripts/disasm.py 0xADDR --end 0xEND` (or
  `objdump -d -Mintel --start-address=0xA --stop-address=0xB resources/th08.exe`).

## 6. MSVC 2002 codegen facts (verified, saves hours)

- Everything is `__fastcall` by default (`/Gr`): do NOT write explicit `__fastcall`;
  2-arg calls → ecx, edx. Methods: ecx=this, spills go **below** named locals.
- Per-TU flags in `scripts/configure.py`: AsciiManager/Ending/EnemyManager/... = `/Od`
  (debug_codegen); Gui/MusicRoom = `/Os`; GameManager/Supervisor = `/Os /Ob1`;
  ResultScreen `/Os /Oi-`; TitleScreen `/Os /Oi- /Ob1`. `/Od` TUs keep dead stores
  and use `push ecx` for a single spill.
- `/Od`: `return ZUN_ERROR(-1)` compiles to `or eax,0xffffffff; jmp <epilogue>`.
  Separate `return`s ⇒ separate `or eax,-1` blocks (helps count `if`s).
- `#pragma var_order(a, b, ...)`: FIRST listed local at `[ebp-4]`, subsequent ones at
  progressively lower addresses (arrays/matrices: base = lowest). Use it to pin stack order.
- Float compares: `fcomp` + `test ah,0x5;jnp` (taken iff ST0<op), `test ah,0x41;jne` = `z<=B`.
- `FSINCOS`: ST0=cos, ST1=sin (first `fstp` = cos). Prefer the project's `sincos()` helper.
- `Float3` has a folded `operator float*()`: `(f32 *)obj` then `[eax+n*4]` indexing.
  Class-returning methods: ecx=this, hidden ret-ptr = FIRST stack arg.
- **Nested-scope (non-function-level) locals are laid out by NAME HASH, NOT declaration
  order** (two compare cycles of evidence: decl-order swaps changed nothing; renames moved
  slots predictably). C1XX keeps a fresh 16-bucket hash table (`hash & 15`) per block
  scope; the pragma plugin only rewrites the FIRST scope it sees (normally the function
  body) — deeper blocks go through vanilla insertion. Bucket walk ascending → offsets
  descending contiguously (first-walked = highest address, closest to ebp). Child scopes
  start below the parent's lowest var; sibling scopes later in source start lower still.
  Practical fix: RENAME the locals so bucket order == original layout (renames don't
  change the binary). Known bucket chains (walk order): `dy < target < t < dx`,
  `v1 < v2 < effect`. Single-letter/simple names collide into predictable pairs; collect
  more chains from compare diffs.
- Compiler temps (sret buffers for `Float3 operator+`, ternary results, the ecx `this`
  spill) land at the BOTTOM of the frame, below named locals — a lone temp at
  `[ebp-0x28]`-style slots under all pragma'd vars usually means an anonymous ternary
  (`cond ? A : B` stored straight into a struct field) or the hidden sret pointer,
  not a missing named local.
- SJIS spell-name literals: write EVERY byte as `\xXX`. `"\x8eE"` parses as hex `0x8ee`
  (greedy escape) and fails with C2022 — `"\x8e\x45"` is the correct form.
- Globals normalize by demangled name; `BSS`/`!BSS` sizes in `config/globals.csv` must
  match the original layout or every reference mismatches.
