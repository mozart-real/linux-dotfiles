#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COLS   110
#define TRAIL_LEN   20
#define TICK_MS     55

static const char *BLUE_RAMP[] = {
    "#e0f2fe","#bae6fd","#7dd3fc","#38bdf8",
    "#0ea5e9","#0284c7","#0369a1","#075985",
    "#0c4a6e","#082f49","#071f35","#061728",
    "#050f1c","#040a14","#03070e","#020508",
    "#010304","#010203","#010102","#000101",
    "#000000"
};
#define RAMP_LEN (int)(sizeof(BLUE_RAMP)/sizeof(BLUE_RAMP[0]))

static const char *CHARS[] = {
    "\xe3\x82\xa2","\xe3\x82\xa4","\xe3\x82\xa6","\xe3\x82\xa8","\xe3\x82\xaa",
    "\xe3\x82\xab","\xe3\x82\xad","\xe3\x82\xaf","\xe3\x82\xb1","\xe3\x82\xb3",
    "\xe3\x82\xb5","\xe3\x82\xb7","\xe3\x82\xb9","\xe3\x82\xbb","\xe3\x82\xbd",
    "\xe3\x83\x8a","\xe3\x83\x8b","\xe3\x83\x8c","\xe3\x83\x8d","\xe3\x83\x8e",
    "0","1","2","3","4","5","6","7","8","9",
    "A","B","C","D","E","F","G","H","Z","X"
};
#define NCHARS (int)(sizeof(CHARS)/sizeof(CHARS[0]))

typedef struct { int w, h, cols, font; } Preset;
static const Preset PRESETS[] = {
    { 160, 240,  11, 10 },
    { 240, 360,  16, 12 },
    { 320, 480,  22, 14 },
    { 480, 600,  32, 14 },
    { 640, 720,  44, 14 },
    { 800, 900,  54, 14 },
    { 960,1080,  64, 14 },
    {1200,1080,  80, 14 },
    {1600,1080, 106, 14 },
};
#define NUM_PRESETS (int)(sizeof(PRESETS)/sizeof(PRESETS[0]))

typedef struct {
    double y, speed;
    int trail[TRAIL_LEN];
    int active;
} Column;

typedef struct {
    GtkWidget *window;
    GtkWidget *area;
    Column     cols[MAX_COLS];
    int col_w, cur_w, cur_h, cur_cols, cur_font;
} App;

static void reset_col(Column *c, int delay) {
    c->y      = -(delay * 18.0);
    c->speed  = 4.0 + (rand() % 60) / 10.0;
    c->active = 1;
    for (int i = 0; i < TRAIL_LEN; i++)
        c->trail[i] = rand() % NCHARS;
}

static void apply_preset(App *app, int idx) {
    if (idx < 0 || idx >= NUM_PRESETS) return;
    const Preset *p = &PRESETS[idx];
    app->cur_w = p->w; app->cur_h = p->h;
    app->cur_cols = p->cols; app->cur_font = p->font;
    app->col_w = p->w / p->cols;
    for (int i = 0; i < p->cols; i++)
        reset_col(&app->cols[i], i * 2 + rand() % 15);

    /* Resetar o minimo pra janela poder encolher */
    gtk_widget_set_size_request(app->window, -1, -1);
    /* Redimensionar via drawing area — janela segue o filho */
    gtk_widget_set_size_request(app->area, p->w, p->h);
    gtk_drawing_area_set_content_width(GTK_DRAWING_AREA(app->area),  p->w);
    gtk_drawing_area_set_content_height(GTK_DRAWING_AREA(app->area), p->h);
    gtk_widget_queue_draw(app->area);
}

static void on_draw(GtkDrawingArea *da, cairo_t *cr,
                    int width, int height, gpointer user_data) {
    App *app = (App *)user_data;
    (void)da; (void)width;

    cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
    cairo_paint(cr);
    cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
    cairo_select_font_face(cr, "JetBrainsMono Nerd Font",
        CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, app->cur_font);

    int cell_h = app->cur_font + 2;
    for (int ci = 0; ci < app->cur_cols; ci++) {
        Column *c = &app->cols[ci];
        if (!c->active) continue;
        int cx = ci * app->col_w + 4;
        for (int ti = 0; ti < TRAIL_LEN; ti++) {
            double cy = c->y - ti * cell_h;
            if (cy < -cell_h || cy > height + cell_h) continue;
            int ri = ti * (RAMP_LEN - 1) / (TRAIL_LEN - 1);
            if (ri >= RAMP_LEN) ri = RAMP_LEN - 1;
            int r, g, b;
            sscanf(BLUE_RAMP[ri] + 1, "%02x%02x%02x", &r, &g, &b);
            double alpha = 1.0 - (ti / (double)TRAIL_LEN) * 0.75;
            cairo_set_source_rgba(cr, r/255.0, g/255.0, b/255.0, alpha);
            if (rand() % 30 == 0) c->trail[ti] = rand() % NCHARS;
            cairo_move_to(cr, cx, cy);
            cairo_show_text(cr, CHARS[c->trail[ti]]);
        }
    }
}

static gboolean on_tick(gpointer user_data) {
    App *app = (App *)user_data;
    int cell_h = app->cur_font + 2;
    for (int ci = 0; ci < app->cur_cols; ci++) {
        Column *c = &app->cols[ci];
        if (!c->active) continue;
        c->y += c->speed;
        if (c->y - TRAIL_LEN * cell_h > app->cur_h + 40)
            reset_col(c, rand() % 30);
    }
    gtk_widget_queue_draw(app->area);
    return G_SOURCE_CONTINUE;
}

static void on_press(GtkGestureClick *gesture, int n, double x, double y, gpointer user_data) {
    (void)n;
    App *app = (App *)user_data;
    GdkEvent    *ev  = gtk_gesture_get_last_event(GTK_GESTURE(gesture), NULL);
    if (!ev) return;
    GdkSurface  *sur = gtk_native_get_surface(GTK_NATIVE(app->window));
    gdk_toplevel_begin_move(GDK_TOPLEVEL(sur), gdk_event_get_device(ev),
                            GDK_BUTTON_PRIMARY, x, y, gdk_event_get_time(ev));
}

static void on_right_click(GtkGestureClick *gesture, int n, double x, double y, gpointer user_data) {
    (void)gesture; (void)n; (void)x; (void)y;
    gtk_window_close(GTK_WINDOW(((App *)user_data)->window));
}

static gboolean on_key(GtkEventControllerKey *ctrl, guint keyval,
                       guint keycode, GdkModifierType mods, gpointer user_data) {
    (void)ctrl; (void)keycode; (void)mods;
    App *app = (App *)user_data;
    if (keyval >= GDK_KEY_1 && keyval <= GDK_KEY_9) {
        apply_preset(app, (int)(keyval - GDK_KEY_1));
        return TRUE;
    }
    if (keyval == GDK_KEY_q || keyval == GDK_KEY_Q) {
        gtk_window_close(GTK_WINDOW(app->window));
        return TRUE;
    }
    return FALSE;
}

static void activate(GtkApplication *gapp, gpointer user_data) {
    App *app = (App *)user_data;
    srand(time(NULL));

    const Preset *def = &PRESETS[2];
    app->cur_w = def->w; app->cur_h = def->h;
    app->cur_cols = def->cols; app->cur_font = def->font;
    app->col_w = def->w / def->cols;
    for (int i = 0; i < app->cur_cols; i++)
        reset_col(&app->cols[i], i * 2 + rand() % 15);

    app->window = gtk_application_window_new(gapp);
    gtk_window_set_title(GTK_WINDOW(app->window), "Animation");
    gtk_window_set_default_size(GTK_WINDOW(app->window), app->cur_w, app->cur_h);
    gtk_window_set_decorated(GTK_WINDOW(app->window), FALSE);
    gtk_window_set_resizable(GTK_WINDOW(app->window), TRUE);
    gtk_widget_set_opacity(app->window, 0.92);

    GtkCssProvider *css = gtk_css_provider_new();
    gtk_css_provider_load_from_string(css,
        "window{background-color:rgba(4,10,20,0.88);"
        "border:1px solid rgba(59,130,246,0.4);border-radius:6px;}");
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),
        GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(css);

    app->area = gtk_drawing_area_new();
    gtk_widget_set_size_request(app->area, app->cur_w, app->cur_h);
    gtk_drawing_area_set_content_width(GTK_DRAWING_AREA(app->area),  app->cur_w);
    gtk_drawing_area_set_content_height(GTK_DRAWING_AREA(app->area), app->cur_h);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(app->area), on_draw, app, NULL);
    gtk_widget_set_focusable(app->area, TRUE);
    gtk_window_set_child(GTK_WINDOW(app->window), app->area);

    GtkGestureClick *lc = GTK_GESTURE_CLICK(gtk_gesture_click_new());
    gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(lc), GDK_BUTTON_PRIMARY);
    g_signal_connect(lc, "pressed", G_CALLBACK(on_press), app);
    gtk_widget_add_controller(app->area, GTK_EVENT_CONTROLLER(lc));

    GtkGestureClick *rc = GTK_GESTURE_CLICK(gtk_gesture_click_new());
    gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(rc), GDK_BUTTON_SECONDARY);
    g_signal_connect(rc, "pressed", G_CALLBACK(on_right_click), app);
    gtk_widget_add_controller(app->area, GTK_EVENT_CONTROLLER(rc));

    GtkEventControllerKey *key = GTK_EVENT_CONTROLLER_KEY(gtk_event_controller_key_new());
    g_signal_connect(key, "key-pressed", G_CALLBACK(on_key), app);
    gtk_widget_add_controller(app->window, GTK_EVENT_CONTROLLER(key));

    g_timeout_add(TICK_MS, on_tick, app);
    gtk_window_present(GTK_WINDOW(app->window));
    gtk_widget_grab_focus(app->area);
}

int main(int argc, char **argv) {
    App app = {0};
    GtkApplication *gapp = gtk_application_new("com.mozart.animation", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(gapp, "activate", G_CALLBACK(activate), &app);
    int status = g_application_run(G_APPLICATION(gapp), argc, argv);
    g_object_unref(gapp);
    return status;
}
