# Changelog

## 2026-09-25

- feat(widget): split `examples/editor_main.c`'s single blocking `main()` into a real,
  independently-callable `editor_widget_*` API (`create`/`begin_frame`/`dispatch_event`/
  `render_frame`/`capture_rgba`/`present`/`tick_autosave`/`should_close`/`shutdown`), so a host
  process can embed this editor as an in-process widget instead of launching it as a separate
  binary (founder real-time: "the editor app fails to launch -- instead of having it launch it
  should pop a widget up on the screen ... dont spawn a separate process deeply integrate it as
  a widget"). Every promoted local kept its exact original name at file scope, so the dispatch/
  render bodies are byte-for-byte unchanged from the original `main()`. Standalone
  `./editor-game [file]` behavior is unchanged (same real, visible window, same 60fps-ish loop),
  now just built out of the same widget functions. Added periodic auto-save
  (`editor_widget_tick_autosave`, every 4s if the buffer changed) on top of the existing F2/
  hover-button manual save, which is untouched. New `examples/editor_widget_test.c` (built via
  `EDITOR_WIDGET_TEST_BUILD`) exercises the full widget API headlessly (Xvfb) with real
  assertions -- create(hidden), inject key/mouse/wheel/text events, render, capture non-blank
  RGBA pixels, autosave, quit -- all passing. New `MODULE.bazel`/`BUILD.bazel` expose this as a
  `cc_library` (`//:editor_widget`) for SHANKPIT's own `apps/lobby` to depend on via a bzlmod
  `local_path_override`; that cross-repo Bazel wiring itself was NOT verified in this session (no
  `bazel` binary in the sandbox) -- build it on a real machine before trusting it. See
  SHANKPIT/apps/lobby/src/editor_widget_bridge.c for the real compositor consuming this.

## 2026-09-22

- New repo, forked from PARENA's own real editor demo (founder real-time: "add the parena editor
  as a stand alone game in SHANKPIT_OS? EDITOR.GAME repo same affordances it currently has for
  saving a file"). Real build chain reproduced and verified (`gen/editor_stdlib_gen.c` +
  `examples/editor_main.c` + `src/`/`runtime/` support files), zero errors/warnings. Live-tested
  headless with a real `.prn` file, real syntax-highlighted rendering confirmed via screenshot.
  See `NORTHSTAR.md`.
