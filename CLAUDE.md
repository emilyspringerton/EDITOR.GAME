# EDITOR.GAME — CLAUDE.md

## What this is

A real, standalone fork of PARENA's own real editor (`PARENA/stdlib/editor/*.prn` +
`PARENA/examples/editor_main.c`), launched as an app from `SHANKPIT/apps/lobby`'s grid alongside
DEADWEIGHT/PITVIPER/IDUNA/REDGARDEN. Same real save affordances PARENA's editor already has — not
reimplemented, just packaged. See `NORTHSTAR.md`.

## Build & run

```sh
sudo apt-get install -y libsdl2-dev libsdl2-ttf-dev
make                    # builds ./editor-game
./editor-game [file]    # opens file, or scratch.prn if omitted
```

Real, verified build chain (see `NORTHSTAR.md`): `gen/editor_stdlib_gen.c` is PARENA-generated
(21 real `.prn` source files, same list `PARENA/examples/editor-demo-sources.txt` names),
`cat`-concatenated with `examples/editor_main.c`, linked against `src/arena.c`/`src/fmt.c`
(symbol-renamed) and `runtime/parena_runtime.c`/`runtime/prnfmt_bridge.c`.

## Regenerating the editor from PARENA

```sh
make regenerate   # needs a sibling ../PARENA checkout with parena already built
```

`gen/editor_stdlib_gen.c` is checked in — never hand-edit it. Regenerate and commit the diff.

## Apple Filing Protocol

```bash
emily apples post -t completion -repo EDITOR.GAME "<title>" "<body with commit hash>"
```

## CHANGELOG Protocol

Append a dated bullet to `CHANGELOG.md` for any meaningful change.

## Related Repos

- `PARENA` — the real source of everything here; this repo has no editor logic of its own.
- `SHANKPIT` — hosts the lobby app-launcher this app is wired into.

## Commit Protocol (standing instruction, monorepo-wide)

Always commit and push completed work immediately. Every commit ends with a blank line then
`session: <tag>` (`emily session current`).

## Core Deps Are PARENA-First (standing, monorepo-wide)

Founder real-time, 2026-10-01: *"always implement core deps in PARENA — when core deps are missing
always implement the core deps in PARENA first."*

- **When a core dependency is missing** (a codec, a protocol client, an inference engine, a
  parser, a data structure — anything this repo's own functionality stands on), implement it in
  PARENA (`PARENA/stdlib/...`) **first**, before building the feature that needs it. Deps first,
  feature second.
- **If PARENA itself can't express the dep yet**, that gap is the real first task: fix or extend
  PARENA (compiler, emitter, or stdlib), with tests, then build the dep on top. Don't route around it.
- **Third-party tools/binaries are stopgaps, not the answer.** Shelling out to or FFI-binding an
  existing tool is allowed only to unblock a demo, and must be labeled as a stopgap in the code and
  in `EMILY/BACKLOG.md` with a PARENA replacement item. (Example: Piper via subprocess for
  MODE_TYLER TTS, 2026-10-01 — stopgap; the PARENA-native synthesis stack is the real work.)
- **Not a license to reimplement the OS.** Core deps = what the product's own behavior depends on.
  Compilers, kernels, system libraries and the like stay as-is; a repo's own CLAUDE.md may record a
  considered, specific exception (same standard as the LZ4 compression convention).
