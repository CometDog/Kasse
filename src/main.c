#include "pebble.h"
#include "pebble-assist.h"
#include "elements.h"
#ifdef PBL_COLOR
	#include "gcolor_definitions."
#endif

/********************************************************* 
 *                      Handle Time                      *
 *********************************************************/

static void update_time() {
	time_t epoch = time(NULL);
	struct tm *t = localtime(&epoch);
	
  hour = t->tm_hour;
  minute = t->tm_minute;
  
	if(clock_is_24h_style() == true) {
		print_time(s_hour_buffer, "%H", t);
	}
	else {
		print_time(s_hour_buffer, "%I", t);
	}
  print_time(s_minute_buffer, "%M", t);
  
  text_layer_set_text(s_hour_label, s_hour_buffer);
  text_layer_set_text(s_minute_label, s_minute_buffer);
}

static void update_movement(Layer *layer, GContext *ctx) {
  
}

/********************************************************* 
 *                      Window Loads                     *
 *********************************************************/

static void main_window_load(Window *window) {
  
  GRect bounds = window_get_bounds(window);

  window_set_background_color(window, GColorBlack);

  s_hour_layer = layer_create(GRect(0,0,134,158));
  s_minute_layer = layer_create(GRect(10,10,134,158));
  
  s_hour_label = text_layer_create(GRect(0,0,10,10));
  s_minute_label = text_layer_create(GRect(0,0,10,10));
  
  text_layer_set_colors(s_hour_label, GColorWhite, GColorBlack);
  text_layer_set_colors(s_minute_label, GColorWhite, GColorBlack);

  text_layer_set_text(s_hour_label, "HH");
  text_layer_set_text(s_minute_label, "MM");
  
  text_layer_set_text_alignment(s_hour_label, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_minute_label, GTextAlignmentCenter);
  
  layer_add_to_window(s_hour_layer, window);
  layer_add_to_window(s_minute_layer, window);
  
  text_layer_add_to_layer(s_hour_label, s_hour_layer);
  text_layer_add_to_layer(s_minute_label, s_minute_layer);
  
  update_time();
}

static void main_window_unload(Window *window) {

  layer_destroy_safe(s_hour_layer);
  layer_destroy_safe(s_minute_layer);
  
  text_layer_destroy_safe(s_hour_label);
  text_layer_destroy_safe(s_minute_label);
}

/********************************************************* 
 *                  (De)Initialization                   *
 *********************************************************/

static void init() {
  s_main_window = window_create();
  window_handlers(s_main_window, main_window_load, main_window_unload);
  window_stack_push(s_main_window, true);
}

static void deinit() {
  window_destroy_safe(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}