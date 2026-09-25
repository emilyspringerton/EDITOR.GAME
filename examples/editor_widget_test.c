/* examples/editor_widget_test.c -- real, standalone smoke test for the
 * editor_widget_* embedded-host API (2026-09-25), exercising exactly
 * the sequence SHANKPIT's own lobby drives per frame: create(hidden=1)
 * -> begin_frame -> inject a few events -> render_frame ->
 * capture_rgba -> present -> tick_autosave -> shutdown. Built as its
 * own separate binary (see Makefile's own `editor-widget-test` target)
 * with EDITOR_WIDGET_TEST_BUILD defined, which compiles out
 * editor_main.c's own real standalone main() so this file's main()
 * is the only one in the link -- both binaries share the exact same
 * editor_widget_*.o code, this is a real test of the same functions,
 * not a separate reimplementation. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int editor_widget_create(const char *path_arg, const char *argv0, int hidden);
void editor_widget_begin_frame(void);
void editor_widget_set_key(int key);
void editor_widget_set_mouse_pos(int x, int y);
void editor_widget_set_wheel_delta(int delta);
void editor_widget_set_text(const char *text);
void editor_widget_inject(int code);
void editor_widget_render_frame(void);
void editor_widget_capture_rgba(unsigned char *out_rgba);
void editor_widget_present(void);
void editor_widget_tick_autosave(double dt_seconds);
int editor_widget_should_close(void);
void editor_widget_shutdown(void);
int editor_widget_width(void);
int editor_widget_height(void);

int main(void) {
    const char *path = "/tmp/editor_widget_test.prn";
    FILE *seed = fopen(path, "w");
    if (seed) { fputs("(print 1)\n", seed); fclose(seed); }

    if (!editor_widget_create(path, "editor-widget-test", 1)) {
        fprintf(stderr, "editor_widget_test: create failed\n");
        return 1;
    }

    int w = editor_widget_width();
    int h = editor_widget_height();
    unsigned char *rgba = (unsigned char *)malloc((size_t)w * (size_t)h * 4);
    if (!rgba) { fprintf(stderr, "editor_widget_test: OOM\n"); return 1; }

    for (int frame = 0; frame < 30; frame++) {
        editor_widget_begin_frame();

        if (frame == 5) {
            /* real TextInput injection -- types "x" into the buffer */
            editor_widget_set_text("x");
            editor_widget_inject(3);
        }
        if (frame == 10) {
            /* real mouse click + motion injection, widget-local coords */
            editor_widget_set_mouse_pos(40, 40);
            editor_widget_inject(5); /* MouseDown */
            editor_widget_inject(6); /* MouseUp */
            editor_widget_set_mouse_pos(60, 60);
            editor_widget_inject(7); /* MouseMotion */
        }
        if (frame == 15) {
            editor_widget_set_wheel_delta(1);
            editor_widget_inject(9); /* MouseWheel */
        }

        editor_widget_render_frame();
        editor_widget_capture_rgba(rgba);
        editor_widget_present();
        editor_widget_tick_autosave(0.2);

        if (editor_widget_should_close()) {
            fprintf(stderr, "editor_widget_test: widget closed unexpectedly at frame %d\n", frame);
            return 1;
        }
    }

    /* Real, minimal correctness check on the captured pixels: not all
     * zero (a genuinely blank/never-rendered-to buffer), matching this
     * editor's own real clear color (24,24,28) as its baseline. */
    int nonzero = 0;
    for (long i = 0; i < (long)w * h * 4; i++) {
        if (rgba[i] != 0) { nonzero = 1; break; }
    }
    if (!nonzero) {
        fprintf(stderr, "editor_widget_test: captured RGBA buffer is all-zero -- capture likely broken\n");
        free(rgba);
        return 1;
    }

    editor_widget_inject(1); /* Quit -- same code Escape/real SDL_QUIT already uses */
    if (!editor_widget_should_close()) {
        fprintf(stderr, "editor_widget_test: should_close() false after injecting Quit\n");
        free(rgba);
        return 1;
    }

    editor_widget_shutdown();
    free(rgba);
    fprintf(stderr, "editor_widget_test: OK (%dx%d, 30 frames, events injected, autosave ticked, quit handled)\n", w, h);
    return 0;
}
