#include <pebble.h>
#include "skiUI.h"
#include "sechandler.h"
#include "accelData.h"
#include "calibrate.h"



Window * my_window;
bool     reading;

// FN Internal

void tick_handler(struct tm *, TimeUnits );
void click_config_provider(void *);
void reading_accel_stop(void);
void reading_accel_start(void);

//
// Load-Unload
//

void window_load(Window *window)
{
//
    createUI(window, click_config_provider);

//Manually call the tick handler when the window is loading
    updateWatch(NULL, MINUTE_UNIT);

}
 
void window_unload(Window *window)
{
// destroy stuff here
    destroyUI(window);
}
 

//
// Button and time handlers
// 


void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
    app_log(APP_LOG_LEVEL_INFO, __FILE__ , __LINE__, "Event timer");
    updateWatch(tick_time, units_changed);
}


void reading_accel_stop() {
    if (reading) 
        accel_deinit();
}

void reading_accel_start() {
    if (!reading) 
        accel_init();
}


void button_calibrate_handler (ClickRecognizerRef recognizer, Window *window)
{
    app_log(APP_LOG_LEVEL_INFO, __FILE__ , __LINE__, "Button 2 pressed");
    reading_accel_stop();
    app_log(APP_LOG_LEVEL_INFO, __FILE__ , __LINE__, "Start Calibration");
    calibrate_accel_UI(window);
}

void switch_reading_handler (ClickRecognizerRef recognizer, Window *window) {
    if (reading) 
        reading_accel_stop();
    else
        reading_accel_start();

    reading = !reading;
   // adjust UI
   app_log(APP_LOG_LEVEL_INFO, __FILE__ , __LINE__, "Reading accel: %i",(int) reading);
}




void reset_counter_handler (ClickRecognizerRef recognizer, Window *window)
{
    app_log(APP_LOG_LEVEL_INFO, __FILE__ , __LINE__, "Button 1 pressed");
}



void click_config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler) reset_counter_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler) button_calibrate_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler) switch_reading_handler);

}



void handle_init(void) {
    my_window = window_create();
    
    tick_timer_service_subscribe(SECOND_UNIT, (TickHandler) tick_handler);
//    tick_timer_service_subscribe(SECOND_UNIT, (TickHandler) sec_handler);

    reading = false;

    window_set_window_handlers(my_window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });

   window_stack_push(my_window, true);
//   window_set_click_config_provider(my_window, click_config_provider);

}

void handle_deinit(void) {
    tick_timer_service_unsubscribe();
    reading_accel_stop();
    window_destroy(my_window);
}

int main(void) {
	  handle_init();
	  app_event_loop();
	  handle_deinit();
}

