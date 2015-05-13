static Window *s_main_window;
static Layer *s_time_layer;

static char s_hour_buffer[] = "XX";
static char s_minute_buffer[] = "XX";

static int8_t hour;
static int8_t minute;

static int minX;
static int minY;
static int hourX;
static int hourY;

static GRect hourbox;
static GRect minutebox;

static int positionsX[] = {6, 17, 28, 39, 50, 61, 72, 83, 94, 105, 116, 127, 138};
static int positionsXI[] = {138, 127, 116, 105, 94, 83, 72, 61, 50, 39, 28, 17, 6};
static int positionsY[] = {7, 17, 27, 36, 46, 55, 65, 75, 84, 93, 103, 113, 122, 132, 141, 151, 160};
static int positionsYI[] = {160, 151, 141, 132, 122, 112, 103, 93, 84, 75, 65, 55, 46, 36, 27, 17, 7};