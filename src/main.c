#include "pebble.h"
#include "libs/pebble-assist.h"
#include "elements.h"
#ifdef PBL_COLOR
	#include "gcolor_definitions.h"
#endif

/********************************************************* 
 *                      Handle Time                      *
 *********************************************************/

static void update_movement(Layer *layer, GContext *ctx) {
  time_t epoch = time(NULL);
	struct tm *t = localtime(&epoch);
  
  int32_t hour_angle = (TRIG_MAX_ANGLE * (((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6);
  int32_t minute_angle = TRIG_MAX_ANGLE * t->tm_min / 60; //  Create minute path

  int hourY = cos_lookup(hour_angle) / TRIG_MAX_RATIO + 84;
  int hourX = sin_lookup(hour_angle) / TRIG_MAX_RATIO + 72;
  int minY = -56 * cos_lookup(minute_angle) / TRIG_MAX_RATIO + 84; // Get the Y position
  int minX = 56 * sin_lookup(minute_angle) / TRIG_MAX_RATIO + 72; // Get the X position
  
  if (hour >= 11 || hour <= 1) {
    hourY = 0;
  }
  else if (hour >= 2 && hour <= 4) {
    hourX = 94;
  }
  else if (hour >= 5 && hour <= 7) {
    hourY = 118;
  }
  else if (hour >= 8 && hour <= 10) {
    hourX = 0;
  }
  
  if (minute >= 55 || minute <= 5) {
    minY = 0;
  }
  else if (minute >= 9 && minute <= 25) {
    minX = 94;
  }
  else if (minute >= 26 && minute <= 35) {
    minY = 118;
  }
  else if (minute >= 36 && minute <= 54) {
    minX = 0;
  }
  
  LOG("Hour Y is %d, Hour X is %d", hourY, hourX);
  LOG("Min Y is %d, Min X is %d", minY, minX);
  
  hourbox = GRect(hourX,hourY,50,50);
  minutebox = GRect(minX,minY,50,50);
  
  graphics_context_set_text_color(ctx, GColorWhite); // Set text color to white
  
  graphics_draw_text(ctx, s_minute_buffer, 
                     fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), 
                     minutebox, 
                     GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL); // Draw the minute

  graphics_draw_text(ctx, s_hour_buffer, 
                     fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), 
                     hourbox, 
                     GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL); // Draw the minute
                     
  layer_mark_dirty(s_time_layer);
}

static void update_time() {
	time_t epoch = time(NULL);
	struct tm *t = localtime(&epoch);
	
  hour = t->tm_hour % 12;
  minute = t->tm_min;
  LOG("Hour is %d", hour);
  
	if(clock_is_24h_style() == true) {
		print_time(s_hour_buffer, "%H", t);
	}
	else {
		print_time(s_hour_buffer, "%I", t);
	}
  print_time(s_minute_buffer, "%M", t);
  
  layer_set_update_proc(s_time_layer, update_movement);
  
}

static void timer_tick(struct tm *tick_time, TimeUnits units_changed) {
  layer_mark_dirty(window_get_root_layer(s_main_window));
}

/********************************************************* 
 *                      Window Loads                     *
 *********************************************************/

static void main_window_load(Window *window) {
  
  GRect bounds = window_get_bounds(window);

  window_set_background_color(window, GColorBlack);

  s_time_layer = layer_create(GRect(0,0,144,168));
  
  layer_add_to_window(s_time_layer, window);
  
  update_time();
}

static void main_window_unload(Window *window) {

  layer_destroy_safe(s_time_layer);
}

/********************************************************* 
 *                  (De)Initialization                   *
 *********************************************************/

static void init() {
  s_main_window = window_create();
  window_handlers(s_main_window, main_window_load, main_window_unload);
  window_stack_push(s_main_window, true);
  
   tick_timer_service_subscribe(MINUTE_UNIT, timer_tick); // Update time each minute
}

static void deinit() {
  window_destroy_safe(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}