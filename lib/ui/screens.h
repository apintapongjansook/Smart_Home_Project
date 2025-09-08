#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *login_page;
    lv_obj_t *home_page;
    lv_obj_t *setting_page;
    lv_obj_t *graph_page;
    lv_obj_t *obj0;
    lv_obj_t *password_txarea;
    lv_obj_t *login_bt;
    lv_obj_t *username_txarea;
    lv_obj_t *keyboard_login;
    lv_obj_t *datetime;
    lv_obj_t *time_display;
    lv_obj_t *day_display;
    lv_obj_t *light_motion;
    lv_obj_t *obj1;
    lv_obj_t *temp_humid;
    lv_obj_t *obj2;
    lv_obj_t *obj3;
    lv_obj_t *obj4;
    lv_obj_t *obj5;
    lv_obj_t *monitor_bt;
    lv_obj_t *graph_bt;
    lv_obj_t *setting_bt;
    lv_obj_t *logout_bt;
    lv_obj_t *monitor_bt_1;
    lv_obj_t *graph_bt_1;
    lv_obj_t *setting_bt_1;
    lv_obj_t *logout_bt_1;
    lv_obj_t *monitor_bt_2;
    lv_obj_t *graph_bt_2;
    lv_obj_t *setting_bt_2;
    lv_obj_t *logout_bt_2;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_LOGIN_PAGE = 1,
    SCREEN_ID_HOME_PAGE = 2,
    SCREEN_ID_SETTING_PAGE = 3,
    SCREEN_ID_GRAPH_PAGE = 4,
};

void create_screen_login_page();
void delete_screen_login_page();
void tick_screen_login_page();

void create_screen_home_page();
void delete_screen_home_page();
void tick_screen_home_page();

void create_screen_setting_page();
void delete_screen_setting_page();
void tick_screen_setting_page();

void create_screen_graph_page();
void delete_screen_graph_page();
void tick_screen_graph_page();

void create_screen_by_id(enum ScreensEnum screenId);
void delete_screen_by_id(enum ScreensEnum screenId);
void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/