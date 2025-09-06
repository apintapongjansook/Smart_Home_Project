#include "event.h"
#include <Arduino.h>


void event_handler(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    //Login Page
    // TextArea ถูก Focused
    if(obj == objects.username_txarea && code == LV_EVENT_FOCUSED) {
        lv_obj_clear_flag(objects.keyboard_login, LV_OBJ_FLAG_HIDDEN);  
        lv_keyboard_set_textarea(objects.keyboard_login, objects.username_txarea);          
    }
    else if(obj == objects.password_txarea && code == LV_EVENT_FOCUSED) {
        lv_obj_clear_flag(objects.keyboard_login, LV_OBJ_FLAG_HIDDEN);  
        lv_keyboard_set_textarea(objects.keyboard_login, objects.password_txarea);          
    }
    else if(obj == objects.keyboard_login && (code == LV_EVENT_READY || code == LV_EVENT_CANCEL)) {
        lv_obj_add_flag(objects.keyboard_login, LV_OBJ_FLAG_HIDDEN);    
    }
    else if(obj == objects.login_bt && code == LV_EVENT_CLICKED){
        const char *username = lv_textarea_get_text(objects.username_txarea);
        const char *password = lv_textarea_get_text(objects.password_txarea);
        if(strcmp(username ,"admin") == 0 && strcmp(password , "1234") == 0) {
            lv_scr_load(objects.main);
        }
    }

}
