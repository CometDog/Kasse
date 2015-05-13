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
  
  if (minute >= 54 || minute <= 6) {
    if (minute >= 54) {
      minute = minute % 6;
    }
    else if (minute <= 6) {
      minute = minute + 6;
    }
    minY = 0;
    minX = positionsX[minute];
  }
  else if (minute >= 7 && minute <= 23) {
    minute = minute - 7;
    minY = positionsY[minute];
    minX = 94;
  }
  else if (minute >= 24 && minute <= 36) {
    minute = minute % 12;
    minY = 128;
    minX = positionsXI[minute];
  }
  else if (minute >= 37 && minute <= 53) {
    minute = minute - 37;
    minY = positionsYI[minute];
    minX = 0;
  }
  hourY = 1;
  hourX = 1;
  
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