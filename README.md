# EDITOR.GAME

A real, standalone fork of [PARENA](../PARENA)'s own real editor (`stdlib/editor/*.prn` +
`examples/editor_main.c`), launched as its own app from SHANKPIT OS's lobby grid. Same real
file-save affordances PARENA's editor already has (F2, hover-reveal Save button,
auto-format-on-save for `.prn` files) — nothing here reimplements them, this repo just packages
the same real build as its own launchable app. See `NORTHSTAR.md` for the full build-chain
writeup and what's verified vs. not yet exercised.

## Build & run

```sh
sudo apt-get install -y libsdl2-dev libsdl2-ttf-dev   # one-time, needs sudo
make                                                    # builds ./editor-game
./editor-game [file.prn]                                # opens file.prn, or scratch.prn if omitted
```

`gen/editor_stdlib_gen.c` is PARENA-generated, checked in as the real source of truth (same
convention `PITVIPER/internal/scrollmod/vterm_mod.c` already established) — `make regenerate`
rebuilds it from a sibling `../PARENA` checkout, never hand-edit it directly.
