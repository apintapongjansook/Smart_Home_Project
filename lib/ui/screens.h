#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *login;
    lv_obj_t *main;
    lv_obj_t *obj0;
    lv_obj_t *password_txarea;
    lv_obj_t *login_bt;
    lv_obj_t *username_txarea;
    lv_obj_t *keyboard_login;
    lv_obj_t *obj1;
    lv_obj_t *obj2;
    lv_obj_t *obj3;
    lv_obj_t *obj4;
    lv_obj_t *obj5;
    lv_obj_t *obj6;
    lv_obj_t *obj7;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_LOGIN = 1,
    SCREEN_ID_MAIN = 2,
};

void create_screen_login();
void delete_screen_login();
void tick_screen_login();

void create_screen_main();
void delete_screen_main();
void tick_screen_main();

void create_screen_by_id(enum ScreensEnum screenId);
void delete_screen_by_id(enum ScreensEnum screenId);
void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/