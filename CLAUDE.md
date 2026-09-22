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
