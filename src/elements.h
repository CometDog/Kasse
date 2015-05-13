static Window *s_main_window;
static TextLayer *s_hour_label, *s_minute_label;
static Layer *s_hour_layer, *s_minute_layer;

static char s_hour_buffer[] = "XX";
static char s_minute_buffer[] = "XX";

static int8_t hour;
static int8_t minute;