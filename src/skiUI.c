
// UI creation and update functions
//
#include <pebble.h>
#include "skiUI.h"
#include "accelData.h"


// class members
static char buffer[] = "00:00";
static char bufINT1[] = "000000";
static char bufINT2[] = "000000";
static char bufINT3[] = "000000";

// UI objects 

TextLayer *text_layer;
InverterLayer *inv_layer;

BitmapLayer *icon_layer;

GBitmap *icon1;
GBitmap *icon2;
GBitmap *icon3;

TextLayer *text_layer_icon1;
TextLayer *text_layer_icon2;
TextLayer *text_layer_icon3;

static GFont *font_Roboto;

// Positions of objects

#define POS_TIME GRect(0, 0, 144, 27)
#define POS_INV GRect(0, 50, 144, 62) 

#define ICONSIZE 40

#define POS_ICONS GRect(0, 30, 40, 128)
#define POS_ICONSKI GRect(0, 0, 40, 40)
#define POS_ICONLIFT GRect(0, 44, 40, 40)
#define POS_ICONREST GRect(0, 88, 40, 40)

#define POS_VALUESKI GRect(50, 30, 80, 30)
#define POS_VALUELIFT GRect(50, 75, 80, 30)
#define POS_VALUEREST GRect(50, 120, 80, 30)


//
// update handler for icons
//



void update_icons (Layer *, GContext* );
void update_icon_counters(void);

//
// Create and position UI elements. 
//

int createUI (Window * my_window) {
    
    font_Roboto = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21 ));
    text_layer = text_layer_create( POS_TIME );
    text_layer_set_background_color(text_layer, GColorBlack);
    text_layer_set_text_color(text_layer, GColorWhite);
    text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
    text_layer_set_font(text_layer, font_Roboto);

    layer_add_child(window_get_root_layer(my_window), (Layer*) text_layer);

// Inverter layer
    inv_layer = inverter_layer_create(POS_INV);
//    layer_add_child(window_get_root_layer(my_window), (Layer*) inv_layer);

// Icons

    icon_layer = bitmap_layer_create (POS_ICONS);
    bitmap_layer_set_background_color(icon_layer, GColorWhite);
    
    icon1 = gbitmap_create_with_resource(RESOURCE_ID_SKIING);
    icon2 = gbitmap_create_with_resource(RESOURCE_ID_INLIFT);
    icon3 = gbitmap_create_with_resource(RESOURCE_ID_RESTING);

    layer_set_update_proc((Layer *)icon_layer, &update_icons);
    layer_add_child(window_get_root_layer(my_window), (Layer*) icon_layer);

// Texts for values

    text_layer_icon1 = text_layer_create( POS_VALUESKI );
    text_layer_set_background_color(text_layer_icon1, GColorWhite);
    text_layer_set_text_color(text_layer_icon1, GColorBlack);
    text_layer_set_text_alignment(text_layer_icon1, GTextAlignmentCenter);
    text_layer_set_font(text_layer_icon1, font_Roboto);
    

    layer_add_child(window_get_root_layer(my_window), (Layer*) text_layer_icon1);

//
    text_layer_icon2 = text_layer_create( POS_VALUELIFT );
    text_layer_set_background_color(text_layer_icon2, GColorWhite);
    text_layer_set_text_color(text_layer_icon2, GColorBlack);
    text_layer_set_text_alignment(text_layer_icon2, GTextAlignmentCenter);
    text_layer_set_font(text_layer_icon2,  font_Roboto);

    layer_add_child(window_get_root_layer(my_window), (Layer*) text_layer_icon2);
//
    text_layer_icon3 = text_layer_create( POS_VALUEREST );
    text_layer_set_background_color(text_layer_icon3, GColorWhite);
    text_layer_set_text_color(text_layer_icon3, GColorBlack);
    text_layer_set_text_alignment(text_layer_icon3, GTextAlignmentCenter);
    text_layer_set_font(text_layer_icon3, font_Roboto);

    layer_add_child(window_get_root_layer(my_window), (Layer*) text_layer_icon3);
    

//

    return 00;
}

void update_icons (Layer *layer, GContext* ctx) {

    // app_log(APP_LOG_LEVEL_INFO, __FILE__ , __LINE__, "Updating icons");
    graphics_draw_bitmap_in_rect(ctx, icon1, POS_ICONSKI);
    graphics_draw_bitmap_in_rect(ctx, icon2, POS_ICONLIFT);
    graphics_draw_bitmap_in_rect(ctx, icon3, POS_ICONREST);
    
    update_icons_counters();
}


void update_icons_counters () {
    int space[3];

    get_data_values(space);

    snprintf(bufINT1, sizeof ("000000"), "%i",space[0] );
    text_layer_set_text(text_layer_icon1, bufINT1);

    snprintf(bufINT2, sizeof ("000000"), "%i",space[1] );
    text_layer_set_text(text_layer_icon2, bufINT2);

    snprintf(bufINT3, sizeof ("000000"), "%i",space[2] );
    text_layer_set_text(text_layer_icon3, bufINT3);

}



int destroyUI (Window * my_window) {
    text_layer_destroy(text_layer);
    text_layer_destroy(text_layer_icon1);
    text_layer_destroy(text_layer_icon2);
    text_layer_destroy(text_layer_icon3);
    inverter_layer_destroy(inv_layer);
    gbitmap_destroy(icon1);
    gbitmap_destroy(icon2);
    gbitmap_destroy(icon3);
    bitmap_layer_destroy(icon_layer);
    return 00;
}


int updateWatch(struct tm * t, TimeUnits units ) {

    time_t temp;

    // app_log(APP_LOG_LEVEL_INFO, __FILE__ , __LINE__, "Event timer %i",(int) units);

    if (t == NULL) {
        temp = time(NULL);
        t = localtime(&temp);
    }
    //Format the buffer string using tick_time as the time source
    strftime(buffer, sizeof("00:00"), "%H:%M", t);
    //Change the TextLayer text to show the new time!
    text_layer_set_text(text_layer, buffer);
    return 00;
}

