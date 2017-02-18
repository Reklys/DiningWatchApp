#include <pebble.h>
#define TEXT_GAP 14
//static char *location;

static Window *menu_window, *location_window;
static MenuLayer *menu_layer;
static TextLayer *menu_text_layer, *location_text_layer;

static char menu_text[32];
static char location_text[32];

typedef struct {
  char name[16]; //name of meal
} MealInfo;

// Array of different meal options

MealInfo meal_array[] = {
  {"Breakfast"},
  {"Lunch"},
  {"Dinner"},
};

static void select_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, 
                            void *callback_context) {
 window_stack_push(location_window, false); 
}
static uint16_t get_sections_count_callback(struct MenuLayer *menulayer, uint16_t section_index, 
                                            void *callback_context) {
  int count = sizeof(meal_array) / sizeof(MealInfo);
  return count;
}

#ifdef PBL_ROUND
static int16_t get_cell_height_callback(MenuLayer *menu_layer, MenuIndex *cell_index, 
                                        void *callback_context) {
  return 60;
}
#endif


static void draw_row_handler(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, 
                             void *callback_context) {
  char* name = meal_array[cell_index->row].name;
//int text_gap_size = TEXT_GAP - strlen(name);
  
  // edge-alignment using space paddiing
 snprintf(menu_text, sizeof(menu_text), "%s", PBL_IF_ROUND_ELSE("", name));
  menu_cell_basic_draw(ctx, cell_layer, PBL_IF_ROUND_ELSE(name, menu_text), 
                       PBL_IF_ROUND_ELSE(menu_text, NULL), NULL);
}
  
static void menu_window_load(Window *window){
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  menu_layer = menu_layer_create(bounds);
  menu_layer_set_callbacks(menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_rows = get_sections_count_callback,
    .get_cell_height = PBL_IF_ROUND_ELSE(get_cell_height_callback, NULL),
    .draw_row = draw_row_handler,
    .select_click = select_callback,
  });
  menu_layer_set_click_config_onto_window(menu_layer, window);
  layer_add_child(window_layer, menu_layer_get_layer(menu_layer));
  
  //s_error_text_layer = text_layer_create((GRect) { .origin = {0, 44}, .size = {bounds.size.w, 60}});
  //text_layer_set_text(s_error_text_layer, "Cannot\nschedule");
  ///text_layer_set_text_alignment(s_error_text_layer, GTextAlignmentCenter);
  //text_layer_set_font(s_error_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  //text_layer_set_text_color(s_error_text_layer, GColorWhite);
  //text_layer_set_background_color(s_error_text_layer, GColorBlack);
  ///layer_set_hidden(text_layer_get_layer(s_error_text_layer), true);
  //layer_add_child(window_layer, text_layer_get_layer(s_error_text_layer));
}

static void menu_window_unload(Window *window) {
  menu_layer_destroy(menu_layer);
}  

static void location_back_handler(ClickRecognizerRef recognizer, void *context) {
  window_stack_pop(true);
}

static void location_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_BACK, location_back_handler);


}

static void location_window_load(Window *window){
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  window_set_click_config_provider(window, location_click_config_provider);
  
  menu_text_layer = text_layer_create(GRect(0,32, bounds.size.w, 20));
  text_layer_set_text(menu_text_layer, "location");
  text_layer_set_text_alignment(menu_text_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(menu_text_layer));
  
  location_text_layer = text_layer_create(GRect(0, 32, bounds.size.w, 20));
  text_layer_set_text(location_text_layer, "location");
  text_layer_set_text_alignment(location_text_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(location_text_layer));

}

static void location_window_unload(Window *window) {
  text_layer_destroy(location_text_layer);
}

static void init(void) {
  
menu_window= window_create();
  window_set_window_handlers(menu_window, (WindowHandlers){
    .load = menu_window_load,
    .unload = menu_window_unload,
  });
location_window = window_create();
  window_set_window_handlers(location_window, (WindowHandlers){
    .load = location_window_load,
    .unload = location_window_unload,
  });
 // window_stack_push(location_window, false);
  window_stack_push(menu_window, false);
}
static void deinit(void) {
  window_destroy(menu_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}