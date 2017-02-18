#include <pebble.h>

static Window *s_window;	
char[2048] s_menu_data = '';

// Keys for AppMessage Dictionary
// These should correspond to the values you defined in appinfo.json/Settings
enum {
	STATUS_KEY = 0,	
	REQUESTMENU_KEY = 1,
	MENUDATA_KEY = 2,
	MENUSIZE_KEY = 3
};


// Write message to buffer & send
static void send_message_request(int val){
	APP_LOG(APP_LOG_LEVEL_DEBUG, "send_message called");
	DictionaryIterator *iter;
	
	app_message_outbox_begin(&iter);
	dict_write_uint8(iter, REQUESTMENU_KEY, val);
	
	dict_write_end(iter);
  app_message_outbox_send();
	APP_LOG(APP_LOG_LEVEL_DEBUG, "send_message complete");
}

// Called when a message is received from PebbleKitJS
static void in_received_handler(DictionaryIterator *received, void *context) {
	Tuple *tuple;
	
	tuple = dict_find(received, STATUS_KEY);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Status: %d", (int)tuple->value->uint32);
		send_message_request(1);
	}
	
	tuple = dict_find(received, REQUESTMENU_KEY);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Message: %s", tuple->value->cstring);
	}
	
	tuple = dict_find(received, MENUDATA_KEY);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received MenuData: %s", tuple->value->cstring);
		s_menu_data = tuple->value->cstring;
	}
  
}

// Called when an incoming message from PebbleKitJS is dropped
static void in_dropped_handler(AppMessageResult reason, void *context) {	
}

// Called when PebbleKitJS does not acknowledge receipt of a message
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
}

static void init(void) {
	s_window = window_create();
	window_stack_push(s_window, true);
	
	// Register AppMessage handlers
	app_message_register_inbox_received(in_received_handler); 
	app_message_register_inbox_dropped(in_dropped_handler); 
	app_message_register_outbox_failed(out_failed_handler);

  // Initialize AppMessage inbox and outbox buffers with a suitable size
  const int inbox_size = 2048;
  const int outbox_size = 256;
	app_message_open(inbox_size, outbox_size);
}

static void deinit(void) {
	app_message_deregister_callbacks();
	window_destroy(s_window);
}

int main( void ) {
	init();
	app_event_loop();
	deinit();
}