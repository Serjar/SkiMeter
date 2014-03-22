#include <pebble.h>
#include "skiUI.h"
#include "sechandler.h"
#include "accelData.h"

Window * my_window;

void tick_handler(struct tm *, TimeUnits );

void window_load(Window *window)
{
//
    createUI(window);
//Manually call the tick handler when the window is loading
    updateWatch(NULL, MINUTE_UNIT);

}
 
void window_unload(Window *window)
{
    accel_deinit();
// destroy stuff here
    destroyUI(window);
}
 

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
    app_log(APP_LOG_LEVEL_INFO, __FILE__ , __LINE__, "Event timer");
    updateWatch(tick_time, units_changed);
}


void handle_init(void) {
    my_window = window_create();
    
    tick_timer_service_subscribe(SECOND_UNIT, (TickHandler) tick_handler);
//    tick_timer_service_subscribe(SECOND_UNIT, (TickHandler) sec_handler);

    accel_init();

    window_set_window_handlers(my_window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });

   window_stack_push(my_window, true);

}

void handle_deinit(void) {
    tick_timer_service_unsubscribe();
    window_destroy(my_window);
}

int main(void) {
	  handle_init();
	  app_event_loop();
	  handle_deinit();
}

