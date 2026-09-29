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

## Fork divergence found and merged (2026-09-29, EDGE.GAME S584 cont.)

The widget-lifecycle refactor above (2026-09-25) and PARENA's own separate 2026-09-10 AVR
current-file/board-profile fix to `compile_and_upload_avr` were built in the two different repos
independently and never reconciled — found live while scoping EDGE.GAME's Phase 2 (the embedded
IDE feedback loop needs both: the widget API to host it, the AVR fix to make its Upload button
actually respect whatever file is open). Checked directly, not assumed: grepped both repos'
`examples/editor_main.c` for the real `editor_widget_create`/`_dispatch_event`/`_render_frame`
function DEFINITIONS — present only in this repo's copy, absent from PARENA's own; PARENA's copy
had `compile_and_upload_avr(Arena *a, const char *current_file)` with board-profile selection via
`EDGE_AVR_UPLOAD_TARGET`, this repo's copy still had the old hardcoded
`compile_and_upload_avr(void)` always targeting `examples/avr/blink.prn` regardless of what the
editor had open. Neither fork had both fixes.

**Merged, not reconciled by picking one side:** ported PARENA's `shell_quote_single` helper and
the full current-file/board-profile-aware `compile_and_upload_avr` onto this repo's own
widget-refactored file scope (`current_file` is EDITOR.GAME's file-scope `path` static, `a` is
its file-scope `Arena a`). One real, deliberate adaptation beyond a straight port: this repo
carries no `examples/avr/` tree, no `tools/avr_touch_reset.py`, and no AVR toolchain of its own
(per this repo's own CLAUDE.md — "this repo has no editor logic of its own"), so the merged
function resolves `current_file` to an absolute path and delegates the actual build+flash via
`make -C ../PARENA <target> AVR_PRN_SOURCE=<abs path>` — the exact same sibling-`../PARENA`-
checkout dependency this repo's own `make regenerate` target already has, not a new kind of
coupling.

**Live-verified, not just code-reviewed:** built a standalone `EDITOR_WIDGET_TEST_BUILD` binary
(same pattern `examples/editor_widget_test.c` already established) that opens a `.prn` file
living OUTSIDE any PARENA-tree path (`/tmp/.../editor_widget_avr_test.prn`), injects a real
MouseMotion (reveal the top bar) then MouseDown/MouseUp on the Upload button's real screen
coordinates, and ran it headless under Xvfb. Confirmed via stderr: the upload command carried the
scratch file's own absolute path (not `examples/avr/blink.prn`), `make -C ../PARENA
avr-blink-upload` really ran, `parena build` → `avr-gcc` → `avr-objcopy` all succeeded against a
real `.hex`, and the only failure was avrdude's own port-open step — the same expected
"no physical hardware in this sandbox" outcome every other AVR target in this monorepo already
has, not a new bug. Both real capabilities (embeddable widget + current-file-aware Upload) now
coexist in one binary for the first time. `make` (the real standalone `./editor-game` build) is
still `-Wall -Wextra -pedantic -Werror` clean.

This closes the real, named Phase 2 prerequisite from `EDGE.GAME/NORTHSTAR.md`'s own "Real,
found-live correction (2026-09-29)" paragraph — EDGE.GAME can now embed this fork's widget API
without losing the AVR fix.

## Related docs

| Doc | Location |
|---|---|
| Real editor host source (unmodified logic) | `PARENA/examples/editor_main.c` |
| Real editor stdlib modules | `PARENA/stdlib/editor/*.prn` |
| Source-file list for the generated build | `PARENA/examples/editor-demo-sources.txt` |
| SHANKPIT OS app-launcher precedent | `SHANKPIT/apps/lobby/src/main.c` (`lobby_launch_app`) |
