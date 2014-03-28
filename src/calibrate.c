#include <pebble.h>
#include "accelData.h"
#include "calibrate.h"

// FN Internal

//void  init_calibrate_layers(Window *);


//
// UI
//

#define POS_CALIBRATE_MSG GRect(0, 0, 144, 127)

static TextLayer * text_calibrate_layer;
static GFont *font;
static Window * calwindow;

static void calwindow_load(Window *window) {
    // Load layers for the window and add them

    app_log(APP_LOG_LEVEL_INFO, __FILE__ , __LINE__, "calwindow load");

    font = fonts_get_system_font(FONT_KEY_GOTHIC_24);

    text_calibrate_layer = text_layer_create( POS_CALIBRATE_MSG );
    text_layer_set_background_color(text_calibrate_layer, GColorWhite);
    text_layer_set_text_color(text_calibrate_layer, GColorBlack);
    text_layer_set_text_alignment(text_calibrate_layer, GTextAlignmentCenter);
    text_layer_set_font(text_calibrate_layer, font);

    layer_add_child(window_get_root_layer(calwindow), (Layer*) text_calibrate_layer);
    text_layer_set_text(text_calibrate_layer, "Calibrate movements now doing 3 times the move you want to measure.");
    calibrate_accel_start();

    app_timer_register( 9000, calibrate_accel_deinit, NULL); 	

}

static void calwindow_unload(Window *window) {
    text_layer_destroy(text_calibrate_layer);
}


void calibrate_accel_UI(Window * window) {

    calwindow = window_create();
    app_log(APP_LOG_LEVEL_INFO, __FILE__ , __LINE__, "calwindow created");

    window_set_background_color(calwindow, GColorWhite);
    window_set_window_handlers(calwindow, (WindowHandlers) {
        .load = calwindow_load,
        .unload = calwindow_unload
    });
    vibes_short_pulse();
    window_stack_push(calwindow, true);
}

//
//
//

void close_window( void * data ){
    window_destroy(window_stack_pop(true));
}


void calibrate_accel_deinit( void * data) {
    int p[3];
    app_log(APP_LOG_LEVEL_INFO, __FILE__ , __LINE__, "End Calibration");
    calibrate_accel_end(data == NULL);
    get_data_values(p);
    if (p[0] != 3) 
        text_layer_set_text(text_calibrate_layer, "Calibrate not successful!! Using Standard");
    else
        text_layer_set_text(text_calibrate_layer, "Calibration successful!! Using custom level")
;

    app_timer_register(3000, close_window, data);
}


