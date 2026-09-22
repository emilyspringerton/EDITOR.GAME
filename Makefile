# EDITOR.GAME Makefile — a real, standalone build of PARENA's own editor (`stdlib/editor/*.prn`)
# + its real host UI (`examples/editor_main.c`), forked out of PARENA into its own SHANKPIT OS
# app. Same real "generated C is the checked-in source of truth" convention PITVIPER's own
# internal/scrollmod/vterm_mod.c already established -- gen/editor_stdlib_gen.c is PARENA-
# generated (`make regenerate`), not hand-edited.

CC ?= gcc
BINARY := editor-game
PRNFMT_RENAME := -Darena_init=pf_arena_init -Darena_alloc=pf_arena_alloc \
	-Darena_strdup=pf_arena_strdup -Darena_free_all=pf_arena_free_all

.PHONY: all clean run regenerate

all: $(BINARY)

# gen/editor_full.c is the same real `cat gen/editor_stdlib_gen.c examples/editor_main.c`
# concatenation PARENA's own `make editor-demo` target uses -- editor_main.c is written to be
# compiled AFTER the generated stdlib code, not to #include it.
gen/editor_full.c: gen/editor_stdlib_gen.c examples/editor_main.c
	cat gen/editor_stdlib_gen.c examples/editor_main.c > gen/editor_full.c

$(BINARY): gen/editor_full.c src/arena.c src/fmt.c runtime/prnfmt_bridge.c runtime/parena_runtime.c
	$(CC) -std=c99 -Wall -Wextra -pedantic -Werror $(PRNFMT_RENAME) -c src/arena.c -o /tmp/editorgame_pf_arena.o
	$(CC) -std=c99 -Wall -Wextra -pedantic -Werror $(PRNFMT_RENAME) -c src/fmt.c -o /tmp/editorgame_pf_fmt.o
	$(CC) -std=c99 -Wall -Wextra -pedantic -Werror $(PRNFMT_RENAME) -Iruntime -Isrc -c runtime/prnfmt_bridge.c -o /tmp/editorgame_pf_bridge.o
	$(CC) -std=c99 -Wall -Wextra -pedantic -Werror -Iruntime gen/editor_full.c runtime/parena_runtime.c \
		/tmp/editorgame_pf_bridge.o /tmp/editorgame_pf_arena.o /tmp/editorgame_pf_fmt.o \
		-o $(BINARY) -lSDL2 -lSDL2_ttf -lm

run: all
	./$(BINARY)

# Regenerates gen/editor_stdlib_gen.c from PARENA's own real stdlib/editor/*.prn sources --
# needs a sibling PARENA checkout with a built `parena` compiler (../PARENA/parena). The list of
# .prn source files this pulls in is PARENA's own examples/editor-demo-sources.txt, not
# duplicated here, so it can't drift out of sync with PARENA's own real editor-demo build.
regenerate:
	cd ../PARENA && ./parena build $$(tr '\n' ' ' < examples/editor-demo-sources.txt) \
		-o ../EDITOR.GAME/gen/editor_stdlib_gen.c

clean:
	rm -f $(BINARY) gen/editor_full.c
