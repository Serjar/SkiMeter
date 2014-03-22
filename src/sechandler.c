
#include <pebble.h>
#include "sechandler.h"
#include "skiUI.h"

void sec_handler(struct tm *tick_time, TimeUnits units_changed)
{
app_log(APP_LOG_LEVEL_INFO, __FILE__ , __LINE__, "Event timer %i",(int) units_changed);

    if (units_changed & SECOND_UNIT) {
        app_log(APP_LOG_LEVEL_INFO, __FILE__ , __LINE__, "Event timer - minute"); 
        updateWatch(tick_time, units_changed);
    }
}

