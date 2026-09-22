# EDITOR.GAME — Northstar

*Written 2026-09-22. Founder real-time: "can we add the parena editor as a stand alone game in
SHANKPIT_OS? EDITOR.GAME repo same affordances it currently has for saving a file."*

## What this is

A real, standalone fork of PARENA's own real editor demo (`PARENA/examples/editor_main.c` + the
real, shipped `stdlib/editor/*.prn` modules it drives — buffer, textmate syntax highlighting,
render, widget, split, spotlight), launched as a fifth app from SHANKPIT OS's lobby grid,
alongside DEADWEIGHT, PITVIPER, IDUNA, and REDGARDEN. Same file-save affordances it already has
in PARENA today — F2 to save, a hover-reveal Save button, real auto-format-on-save for `.prn`
files — none of that was rebuilt, it's the real, existing behavior, just packaged as its own
launchable app instead of living only inside PARENA's own `examples/` dir.

## Real build chain, checked and run, not assumed

`PARENA/examples/editor_main.c` is the real host/UI driver; the actual editor logic (buffer
model, undo, TextMate-grammar syntax highlighting, rendering, split panes, spotlight search) is
`stdlib/editor/*.prn`, compiled through the real `parena build` pipeline into C
(`PARENA/examples/editor-demo-sources.txt` names the exact 21-file list, PARENA's own single
source of truth for it — CI reads the same file, per that Makefile target's own comment about a
past drift bug), then `cat`-concatenated with `editor_main.c` and linked against `src/arena.c`/
`src/fmt.c` (PARENA's own text-formatting arena allocator, symbol-renamed via `-D` flags to avoid
colliding with the generated code's own `arena_*` symbols) and `runtime/parena_runtime.c`/
`runtime/prnfmt_bridge.c`.

This repo forks that real chain: `gen/editor_stdlib_gen.c` is the PARENA-generated output,
checked in as the real source of truth (same convention `PITVIPER/internal/scrollmod/vterm_mod.c`
already established — regenerate via `make regenerate`, never hand-edit), `examples/editor_main.c`
and the `src/`/`runtime/` support files are plain copies. `make all` reproduces PARENA's own real
`make editor-demo` build exactly, standalone, no PARENA checkout needed at build time (only at
`make regenerate` time, which needs a sibling `../PARENA` with its own `parena` compiler built).

**Verified for real, not assumed:** built PARENA's own `editor-demo` target directly first to
confirm the recipe (`cd PARENA && make editor-demo`, clean, zero errors). Forked the exact same
recipe here, built `editor-game` standalone, zero errors, zero warnings (`-Wall -Wextra -pedantic
-Werror`). Ran it headless (Xvfb) with a real `.prn` file passed as `argv[1]`
(`./editor-game /tmp/test_open.prn`) and screenshotted it: real file content
(`(defn hello [] : I32 42)`) rendered with real TextMate syntax highlighting (the numeric literal
`42` in a distinct color) — genuine, live editor behavior, not a blank window.

## What's NOT done (named, not hidden)

- Save itself (F2 / the hover-reveal Save button) was not interactively exercised in this pass
  (would need simulated keyboard/mouse input inside the headless X session) — the underlying
  `do_save`/`save_to_file` code is unchanged from PARENA's own real, already-shipped
  implementation, not reimplemented here, so this is a real, low-risk gap, not an unknown one.
- Not wired into `SHANKPIT/apps/lobby` yet as of this doc's own first commit — see the repo's own
  git log / SHANKPIT's own CHANGELOG for whether that's landed since.
- No IDUNA/brand integration of any kind — this is PARENA's own real editor, unmodified visually,
  not reskinned to match IDUNA.GAME's own Solarized Light work.
- `stdlib/editor/*.prn`'s own upstream changes in PARENA won't automatically appear here —
  `make regenerate` has to be run and the result committed by hand, same real "generated file can
  drift from its source until someone regenerates it" tradeoff every other checked-in-generated-
  code precedent in this monorepo already carries.

## Related docs

| Doc | Location |
|---|---|
| Real editor host source (unmodified logic) | `PARENA/examples/editor_main.c` |
| Real editor stdlib modules | `PARENA/stdlib/editor/*.prn` |
| Source-file list for the generated build | `PARENA/examples/editor-demo-sources.txt` |
| SHANKPIT OS app-launcher precedent | `SHANKPIT/apps/lobby/src/main.c` (`lobby_launch_app`) |
